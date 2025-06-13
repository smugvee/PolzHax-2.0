#include "pch.h"

#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")
#include "CrashLogger.h"
#include "menu.h"
#include "state.h"
#include "utils.hpp"
#include "patching.h"
#include "imgui-hook.hpp"

#include "PlayLayer.h"
#include "PauseLayer.h"
#include "EditorUI.h"
#include "ColorSelectPopup.h"
#include "CustomizeObjectLayer.h"
#include "InfoLayer.h"
#include "EditorOptionsLayer.h"
#include "LevelEditorLayer.h"
#include "EndLevelLayer.h"
#include "EditorPauseLayer.h"
#include "HSVWidgetPopup.h"
#include "LevelInfoLayer.h"
#include "PlayerObject.h"
#include "FindLevelPopup.h"
#include "SetGroupIDLayer.h"

#include "LevelShare.h"
#include "nfd.h"

void(__thiscall* fpMainLoop)(cocos2d::CCDirector* self);
void __fastcall hkMainLoop(cocos2d::CCDirector* self)
{
	ImGuiHook::poll(self->getOpenGLView());
	fpMainLoop(self);
}

bool(__thiscall* GameManager_isIconUnlocked)(gd::GameManager*, int, int);
bool __fastcall GameManager_isIconUnlockedH(gd::GameManager* self, void*, int id, int type) {
	if (setting().onIcons) return true;
	else return GameManager_isIconUnlocked(self, id, type);
}

bool(__thiscall* GameManager_isColorUnlocked)(gd::GameManager*, int, int);
bool __fastcall GameManager_isColorUnlockedH(gd::GameManager* self, void*, int id, unsigned int type) {
	if (setting().onIcons) return true;
	else return GameManager_isColorUnlocked(self, id, type);
}

inline void(__thiscall* CCParticleSystem_initWithTotalParticles)(CCParticleSystem*, unsigned int);
void __fastcall CCParticleSystem_initWithTotalParticlesH(CCParticleSystem* self, void*, unsigned int amt) {
	if (setting().onNoParticles) return CCParticleSystem_initWithTotalParticles(self, 0);
	else return CCParticleSystem_initWithTotalParticles(self, amt);
}

bool(__thiscall* CCKeyboardDispatcher_dispatchKeyboardMSG)(cocos2d::CCKeyboardDispatcher* self, int key, bool down);
void __fastcall CCKeyboardDispatcher_dispatchKeyboardMSGH(CCKeyboardDispatcher* self, void* edx, int key, bool down) {
	auto pl = gd::GameManager::sharedState()->getPlayLayer();
	if (down) {
		if ((key == 'R') && setting().onRetryKeybind) {
			if (pl) {
				if (!pl->m_hasCompletedLevel) {
					pl->resetLevel();
					if (layers().PauseLayerObject)
					{
						layers().PauseLayerObject->removeMeAndCleanup();
						layers().PauseLayerObject = nullptr;
					}
					pl->resume();
					return;
				}
			}
		}
		if (pl) {
			if (setting().onStartPosSwitcher && !pl->m_hasCompletedLevel) {
				switch (key)
				{
				case KEY_Q:
				case KEY_Left:
				case KEY_A:
					PlayLayer::onPrevStartPos();
					std::cout << "PrevStartPos" << std::endl;
					break;
				case KEY_E:
				case KEY_Right:
				case KEY_D:
					PlayLayer::onNextStartPos();
					std::cout << "NextStartPos" << std::endl;
					break;
				}
			}
		}
	}
	CCKeyboardDispatcher_dispatchKeyboardMSG(self, key, down);
}

void(__thiscall* CCTransitionScene_initWithDuration)(CCTransitionScene*, float, CCScene*);
void __fastcall CCTransitionScene_initWithDurationH(CCTransitionScene* self, void* edx, float duration, CCScene* scene) {
	if (setting().onNoTransition) return CCTransitionScene_initWithDuration(self, 0.f, scene);
	else CCTransitionScene_initWithDuration(self, duration, scene);
}

void(__thiscall* RingObject_spawnCircle)(gd::GameObject*);
void __fastcall RingObject_spawnCircleH(gd::GameObject* self) {
	if (!setting().onNoOrbRing) return RingObject_spawnCircle(self);
}

namespace gd {
	class EditLevelLayer : public CCLayer {
	public:
		GJGameLevel* level() {
			return from<GJGameLevel*>(this, 0x12c);
		}
		static auto scene(GJGameLevel* level) {
			return reinterpret_cast<cocos2d::CCScene * (__fastcall*)(GJGameLevel*)>(base + 0x56f00)(level);
		}
	};

	class LevelSettingsLayer : public FLAlertLayer {
	public:
	};
}

class ImportExportCB {
public:
	void onExportLevelELL(CCObject* obj) {
		auto* const level = reinterpret_cast<gd::EditLevelLayer*>(reinterpret_cast<CCNode*>(obj)->getParent()->getParent())->level();
		nfdchar_t* path = nullptr;
		if (NFD_SaveDialog("gmd", nullptr, &path) == NFD_OKAY) {
			std::ofstream file(path);
			dump_level(level, file);
			free(path);
			gd::FLAlertLayer::create(nullptr, "Success", "OK", nullptr, "The level has been saved.")->show();
		}
	}
	void onImportLevelLBL(CCObject* obj) {
		nfdchar_t* path = nullptr;
		if (NFD_OpenDialog("gmd", nullptr, &path) == NFD_OKAY) {
			std::ifstream file(path);
			auto* const level = import_level(file);
			free(path);
			if (!level) {
				gd::FLAlertLayer::create(nullptr, "Error", "OK", nullptr, "Failed to import.")->show();
				return;
			}
			auto scene = gd::EditLevelLayer::scene(level);
			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(.5f, scene));
		}
	}
};

class EditLevelLayerCB : public gd::EditLevelLayer, gd::FLAlertLayerProtocol {
public:
	void onResetPercentage(CCObject*) {
		gd::GJGameLevel* level = from<gd::GJGameLevel*>(this, 0x12c);
		level->m_normalPercentRand1 = 0;
		level->m_normalPercentRand2 = 0;
		level->m_normalPercent = 0;
		level->m_practicePercent = 0;
		level->m_isVerified = false;
	}
};

bool(__thiscall* EditLevelLayer_init)(gd::EditLevelLayer*, gd::GJGameLevel*);
bool __fastcall EditLevelLayer_initH(gd::EditLevelLayer* self, void*, gd::GJGameLevel* level) {
	if (!EditLevelLayer_init(self, level)) return false;

	auto director = CCDirector::sharedDirector();

	auto shareMenu = CCMenu::create();

	auto btn_spr = CCSprite::createWithSpriteFrameName("GJ_downloadBtn_001.png");
	if (!btn_spr->initWithFile("BE_Export_File.png")) {
		btn_spr->initWithSpriteFrameName("GJ_downloadBtn_001.png");
	}
	auto button = gd::CCMenuItemSpriteExtra::create(btn_spr, nullptr, self, menu_selector(ImportExportCB::onExportLevelELL));
	button->setPosition({ -30, +30 });

	shareMenu->setZOrder(1);
	shareMenu->setPosition({ director->getScreenRight(), director->getScreenBottom() });
	shareMenu->addChild(button);
	self->addChild(shareMenu);

	auto mainMenu = from<CCMenu*>(self, 0x128);

	if (setting().onDeveloperMode) {
		auto onResetPercentageSpr = CCSprite::createWithSpriteFrameName("GJ_replayBtn_001.png");
		auto onResetPercentage = gd::CCMenuItemSpriteExtra::create(onResetPercentageSpr, self, menu_selector(EditLevelLayerCB::onResetPercentage));
		onResetPercentageSpr->setOpacity(100);
		onResetPercentage->setPositionX(-180);
		mainMenu->addChild(onResetPercentage);
	}

	return true;
}

class LevelBrowserLayerCB : public gd::LevelBrowserLayer {
public:
	void onSearch(CCObject*) {
		auto popup = FindLevelPopup::create();
		popup->setUserData(this);
		popup->show();
		//gd::GJSearchObject* searchObject = from<gd::GJSearchObject*>(this, 0x13c);
		//searchObject->m_searchQuery = "s";
		//this->loadPage(searchObject);
	}
};

bool(__thiscall* LevelBrowserLayer_init)(gd::LevelBrowserLayer*, gd::GJSearchObject*);
bool __fastcall LevelBrowserLayer_initH(gd::LevelBrowserLayer* self, void*, gd::GJSearchObject* obj) {
	if (!LevelBrowserLayer_init(self, obj)) return false;

	if (obj->m_type == gd::SearchType::MyLevels) {
		auto menu = CCMenu::create();
		menu->setPosition(CCDirector::sharedDirector()->getWinSize().width - 30.f, 90);
		self->addChild(menu);

		auto btn_spr = CCSprite::createWithSpriteFrameName("GJ_downloadBtn_001.png");
		if (!btn_spr->initWithFile("BE_Import_File.png")) {
			btn_spr->initWithSpriteFrameName("GJ_downloadBtn_001.png");
		}

		auto button = gd::CCMenuItemSpriteExtra::create(btn_spr, nullptr, self, menu_selector(ImportExportCB::onImportLevelLBL));

		menu->setZOrder(1);
		menu->addChild(button);

		if (setting().onDeveloperMode) {
			auto button2 = gd::CCMenuItemSpriteExtra::create(CCSprite::create("GJ_button_01.png"), self, menu_selector(LevelBrowserLayerCB::onSearch));
			menu->addChild(button2);
			button2->setPosition(-100, 100);
		}
	}

	return true;
}

void(__thiscall* CCTextInputNode_updateLabel)(gd::CCTextInputNode*, std::string);
void __fastcall CCTextInputNode_updateLabelH(gd::CCTextInputNode* self, void*, std::string string) {
	if (setting().onTextLength)
		self->m_maxLabelLength = 99999;

	CCTextInputNode_updateLabel(self, string);

	if (setting().onCharFilter) {
		self->m_filter = "abcdefghijklmnopqrstuvwxyz"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"0123456789!@#$%^&*()-=_+"
			"`~[]{}/?.>,<\\|;:'\""
			" ";
		CCTextInputNode_updateLabel(self, std::move(string));
	}
}

class LevelSettingsLayerCB : public gd::LevelSettingsLayer {
};

inline bool(__thiscall* LevelSettingsLayer_init)(gd::LevelSettingsLayer*, gd::LevelSettingsObject*, gd::LevelEditorLayer*);
bool __fastcall LevelSettingsLayer_initH(gd::LevelSettingsLayer* self, void*, gd::LevelSettingsObject* obj, gd::LevelEditorLayer* lel) {
	if (!LevelSettingsLayer_init(self, obj, lel)) return false;

	

	return true;
}

inline void(__thiscall* CCCircleWave_draw)(gd::CCCircleWave*);
void __fastcall CCCircleWave_drawH(gd::CCCircleWave* self) {
	if (!setting().onNoEffectCircle) CCCircleWave_draw(self);
}

inline void(__thiscall* HardStreak_updateStroke)(gd::HardStreak*, float);
void __fastcall HardStreak_updateStrokeH(gd::HardStreak* self, void*, float dt) {
	if (setting().onWavePulseSize) self->m_pulseSize = setting().wavePulseSize;
	HardStreak_updateStroke(self, dt);
}

inline void(__thiscall* AudioEffectsLayer_updateTweenAction)(gd::AudioEffectsLayer*, float, char const*);
void __fastcall AudioEffectsLayer_updateTweenActionH(gd::AudioEffectsLayer* self, void*, float idk, char const* idk2) {
	AudioEffectsLayer_updateTweenAction(self, idk, idk2);
	if (setting().onNoPulse) from<float>(self, 0x1ac) = 1.f;
}

inline void(__thiscall* FMODAudioEngine_update)(gd::FMODAudioEngine*, float);
void __fastcall FMODAudioEngine_updateH(gd::FMODAudioEngine* self, void*, float dt) {
	FMODAudioEngine_update(self, dt);
	if (setting().onNoPulse) self->m_pulse1 = 1.f;
}

inline void(__thiscall* EffectGameObject_triggerObject)(gd::EffectGameObject*);
void __fastcall EffectGameObject_triggerObjectH(gd::EffectGameObject* self) {
	if (setting().onShowLayout) {
		switch (self->m_objectID)
		{
		case 29:
		case 30:
		case 105:
		case 744:
		case 899:
		case 900:
		case 915:
		case 1006:
		case 1007:
			return; break;
		}
	}
	EffectGameObject_triggerObject(self);
}

class CustomSongWidgetCB : public gd::CustomSongWidget {
public:
	void onCopySongID(CCObject*) {
		clipboard::write(std::to_string(this->m_songID));
	}
};

inline bool(__thiscall* CustomSongWidget_init)(gd::CustomSongWidget*, gd::SongInfoObject*, gd::LevelSettingsObject*, bool, bool, bool, bool, bool);
bool __fastcall CustomSongWidget_initH(gd::CustomSongWidget* self, void*, gd::SongInfoObject* songInfo, gd::LevelSettingsObject* levelSettings, bool p0, bool p1, bool p2, bool p3, bool p4) {
	if (!CustomSongWidget_init(self, songInfo, levelSettings, p0, p1, p2, p3, p4)) return false;

	if (!self->m_isRobtopSong && self->m_moreBtn->isVisible()) {
		auto onCopySongIDSpr = gd::ButtonSprite::create("Copy", 0xdc, false, "bigFont.fnt", "GJ_button_04.png", 25.f, .6f);
		onCopySongIDSpr->setScale(.7f);
		auto onCopySongID = gd::CCMenuItemSpriteExtra::create(onCopySongIDSpr, self, menu_selector(CustomSongWidgetCB::onCopySongID));
		onCopySongID->setPosition(self->m_moreBtn->getPositionX() + 50.f, self->m_moreBtn->getPositionY());
		self->m_buttonMenu->addChild(onCopySongID, 0, 333);
	}

	return true;
}

inline void(__thiscall* CustomSongWidget_updateSongInfo)(gd::CustomSongWidget*);
void __fastcall CustomSongWidget_updateSongInfoH(gd::CustomSongWidget* self) {
	CustomSongWidget_updateSongInfo(self);
	auto onCopySongID = static_cast<gd::CCMenuItemSpriteExtra*>(self->m_buttonMenu->getChildByTag(333));
	std::cout << onCopySongID << std::endl;
	if (onCopySongID) {
		onCopySongID->setPosition(self->m_moreBtn->getPositionX() + 50.f, self->m_moreBtn->getPositionY());
	}
}

void(__thiscall* AppDelegate_trySaveGame)(gd::AppDelegate*);
void __fastcall AppDelegate_trySaveGameH(gd::AppDelegate* self) {
	if (setting().onAutoSave)
	{
		setting().saveState();
	}
	AppDelegate_trySaveGame(self);
	std::cout << "Saved..." << std::endl;
}

DWORD WINAPI my_thread(void* hModule) {
	sequence_patch((uint32_t)gd::base + 0x558db, { 0xb8, 0x01, 0x00, 0x00, 0x00, 0x90, 0x90 }); // Play music button in level page
	sequence_patch((uint32_t)gd::base + 0x38a85, { 0x6a, 0x00 }); // 2.2 color format (better texture quality)
	sequence_patch((uint32_t)gd::base + 0xfefdc, { 0x90, 0x90 }); // Lines editor fix
	sequence_patch((uint32_t)gd::base + 0x1d13f6, { 0x0a }); // "EL: %s\n"
	sequence_patch((uint32_t)gd::base + 0x55aa9, { 0x00, 0x00, 0x00, 0x43 }); // CustomSongWidget m_artistLabel limitLabelWidth(128.f, ...)
	sequence_patch((uint32_t)gd::base + 0x18bcaf, { 0xc7, 0x04, 0x24, 0x00, 0x00, 0x11, 0x43, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }); // SetGroupIDLayer::init move next free button to -145.f
	sequence_patch((uint32_t)gd::base + 0xd9cae, { 0xf0, 0xcd, 0x60, 0x00 }); // EditorOptionsLayer Buttons Rows node
	sequence_patch((uint32_t)gd::base + 0xd9a48, { 0xf0, 0xcd, 0x60, 0x00 }); // EditorOptionsLayer Buttons Per Row node
	sequence_patch((uint32_t)gd::base + 0x5b803, { 0x00, 0x00, 0xa0, 0x42 }); // EditorPauseLayer::customSetup Keys button
	sequence_patch((uint32_t)gd::base + 0x5b882, { 0x00, 0x00, 0xf0, 0x42 }); // EditorPauseLayer::customSetup Options button

	//AllocConsole();
	//freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);

	if (MH_Initialize() != MH_OK) {
		FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(hModule), 0);
	}

	auto cocos = GetModuleHandleA("libcocos2d.dll");
	auto cocos_ext = GetModuleHandleA("libExtensions.dll");

	PlayLayer::mem_init();
	PauseLayer::mem_init();
	EditorPauseLayer::mem_init();
	EditorUI::mem_init();
	ColorSelectPopup::mem_init();
	SetupPulsePopup::mem_init();
	CustomizeObjectLayer::mem_init();
	EditButtonBar::mem_init();
	LevelEditorLayer::mem_init();
	//HSVWidgetPopup::mem_init();
	LevelInfoLayer::mem_init();
	PlayerObject::mem_init();
	SetGroupIDLayer::mem_init();

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9afc0), GameManager_isIconUnlockedH, reinterpret_cast<void**>(&GameManager_isIconUnlocked));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9b2a0), GameManager_isColorUnlockedH, reinterpret_cast<void**>(&GameManager_isColorUnlocked));

	MH_CreateHook(reinterpret_cast<void*>(GetProcAddress(cocos, "?initWithTotalParticles@CCParticleSystem@cocos2d@@UAE_NI@Z")), CCParticleSystem_initWithTotalParticlesH, reinterpret_cast<void**>(&CCParticleSystem_initWithTotalParticles));

	MH_CreateHook(
		reinterpret_cast<void*>(GetProcAddress(cocos, "?dispatchKeyboardMSG@CCKeyboardDispatcher@cocos2d@@QAE_NW4enumKeyCodes@2@_N@Z")),
		reinterpret_cast<void**>(&CCKeyboardDispatcher_dispatchKeyboardMSGH),
		reinterpret_cast<void**>(&CCKeyboardDispatcher_dispatchKeyboardMSG));

	MH_CreateHook(reinterpret_cast<void*>(GetProcAddress(cocos, "?initWithDuration@CCTransitionScene@cocos2d@@UAE_NMPAVCCScene@2@@Z")), CCTransitionScene_initWithDurationH, reinterpret_cast<void**>(&CCTransitionScene_initWithDuration));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x156760), OptionsLayer_onSecretVaultH, reinterpret_cast<void**>(&OptionsLayer_onSecretVault));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x19d270), RingObject_spawnCircleH, reinterpret_cast<void**>(&RingObject_spawnCircle));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x56fa0), EditLevelLayer_initH, reinterpret_cast<void**>(&EditLevelLayer_init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xea4c0), LevelBrowserLayer_initH, reinterpret_cast<void**>(&LevelBrowserLayer_init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x1dac0), CCTextInputNode_updateLabelH, reinterpret_cast<void**>(&CCTextInputNode_updateLabel));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe5680), InfoLayer::initH, reinterpret_cast<void**>(&InfoLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x102270), LevelSettingsLayer_initH, reinterpret_cast<void**>(&LevelSettingsLayer_init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xd9900), EditorOptionsLayer::initH, reinterpret_cast<void**>(&EditorOptionsLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xfe2e0), DrawGridLayer::drawH, reinterpret_cast<void**>(&DrawGridLayer::draw));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x13bf0), CCCircleWave_drawH, reinterpret_cast<void**>(&CCCircleWave_draw));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xe4680), HardStreak_updateStrokeH, reinterpret_cast<void**>(&HardStreak_updateStroke));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x39950), AudioEffectsLayer_updateTweenActionH, reinterpret_cast<void**>(&AudioEffectsLayer_updateTweenAction));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x20560), FMODAudioEngine_updateH, reinterpret_cast<void**>(&FMODAudioEngine_update));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x74c00), EndLevelLayer::customSetupH, reinterpret_cast<void**>(&EndLevelLayer::customSetup));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x19e250), EffectGameObject_triggerObjectH, reinterpret_cast<void**>(&EffectGameObject_triggerObject));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x54010), CustomSongWidget_initH, reinterpret_cast<void**>(&CustomSongWidget_init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x55590), CustomSongWidget_updateSongInfoH, reinterpret_cast<void**>(&CustomSongWidget_updateSongInfo));

	MH_CreateHook(
		reinterpret_cast<void*>(gd::base + 0x392a0),
		reinterpret_cast<void**>(&AppDelegate_trySaveGameH),
		reinterpret_cast<void**>(&AppDelegate_trySaveGame));

	setup_imgui_menu();

	MH_EnableHook(MH_ALL_HOOKS);

	MH_CreateHook(reinterpret_cast<LPVOID>(reinterpret_cast<uintptr_t>(cocos) + 0xfde30), hkMainLoop, reinterpret_cast<LPVOID*>(&fpMainLoop));

	return true;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0x1000, my_thread, hModule, 0, 0);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	setupExceptionHandler();
	return TRUE;
}

