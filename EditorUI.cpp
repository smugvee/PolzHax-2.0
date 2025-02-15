#define NOMINMAX
#include "EditorUI.h"
#include "moveForCommand.h"
#include "utils.hpp"
#include "GlobalClipboard.h"

gd::EditorPauseLayer* m_editorPauseLayer{ nullptr };

bool __fastcall EditorUI::initH(gd::EditorUI* self, void*, gd::LevelEditorLayer* editorLayer) {
	if (!EditorUI::init(self, editorLayer)) return false;

	loadClipboard(self);
	std::cout << self->m_editorLayer << std::endl;

	return true;
}

void __fastcall EditorUI::dtorH(gd::EditorUI* self) {
	saveClipboard(self);
	EditorUI::dtor(self);
}

void __fastcall EditorUI::onCopyH(gd::EditorUI* self, void*, CCObject* obj) {
	EditorUI::onCopy(self, obj);
	clipboard::write(self->m_clipboard);
}

void __fastcall EditorUI::scrollWheelH(gd::EditorUI* _self, void* edx, float dy, float dx) { // From MatsHacks
	auto self = reinterpret_cast<gd::EditorUI*>(reinterpret_cast<uintptr_t>(_self) - 0xf8);
	auto layer = reinterpret_cast<gd::LevelEditorLayer*>(self->getParent())->m_objectLayer;
	auto zoom = layer->getScale();

	//static_assert(offsetof(CCDirector, m_pKeyboardDispatcher) == 0x4c, "it wrong!");
	auto kb = CCDirector::sharedDirector()->m_pKeyboardDispatcher;
	if (kb->getControlKeyPressed()) {
		zoom = static_cast<float>(std::pow(2.71828182845904523536, std::log(std::max(zoom, 0.001f)) - dy * 0.01f));
		// zoom limit
		zoom = std::max(zoom, 0.01f);
		zoom = std::min(zoom, 1000000.f);
		self->updateZoom(zoom);
		reinterpret_cast<gd::LevelEditorLayer*>(self->getParent())->updateGroundWidth();
	}
	else if (kb->getShiftKeyPressed()) {
		layer->setPositionX(layer->getPositionX() - dy * 1.f);
	}
	else {
		EditorUI::scrollWheel(_self, dy, dx);
	}
}

class SaveLevelProtocol : public gd::FLAlertLayerProtocol {
protected:
	void FLAlert_Clicked(gd::FLAlertLayer* layer, bool btn2) {
		if (btn2) {
			m_editorPauseLayer->saveLevel();
		}
	}
};

SaveLevelProtocol saveLevelProtocol;

void EditorPauseLayer::Callback::onSaveLevel(CCObject*) {
	gd::FLAlertLayer::create(&saveLevelProtocol, "Save", "NO", "YES", 300.f, "<cy>Save</c> the level?")->show();
}

void EditorPauseLayer::Callback::onPasteString(CCObject*) {
	if (gd::GameManager::sharedState()->getLevelEditorLayer()) {
		auto string = clipboard::read();
		gd::GameManager::sharedState()->getLevelEditorLayer()->m_editorUI->pasteObjects(string);
		gd::GameManager::sharedState()->getLevelEditorLayer()->m_editorUI->updateButtons();
	}
}

void __fastcall EditorPauseLayer::customSetupH(gd::EditorPauseLayer* self) {
	EditorPauseLayer::customSetup(self);
	m_editorPauseLayer = self;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto mainMenu = static_cast<CCMenu*>(self->getChildren()->objectAtIndex(0));
	auto actionsMenu = static_cast<CCMenu*>(self->m_audioOnBtn->getParent());

	auto onResume = static_cast<gd::CCMenuItemSpriteExtra*>(mainMenu->getChildren()->objectAtIndex(0));
	onResume->setPositionY(84.f);
	auto onSaveAndPlay = static_cast<gd::CCMenuItemSpriteExtra*>(mainMenu->getChildren()->objectAtIndex(1));
	onSaveAndPlay->setPositionY(42.f);
	auto onSaveAndExit = static_cast<gd::CCMenuItemSpriteExtra*>(mainMenu->getChildren()->objectAtIndex(2));
	onSaveAndExit->setPositionY(0.f);
	auto onExitNoSave = static_cast<gd::CCMenuItemSpriteExtra*>(mainMenu->getChildren()->objectAtIndex(3));
	onExitNoSave->setPositionY(-84.f);

	auto onSaveSpr = gd::ButtonSprite::create("Save", 0xb4, true, "goldFont.fnt", "GJ_button_01.png", 28.f, .8f);
	auto onSave = gd::CCMenuItemSpriteExtra::create(onSaveSpr, nullptr, self, menu_selector(EditorPauseLayer::Callback::onSaveLevel));
	onSave->setPositionY(-42.f);

	mainMenu->addChild(onSave);

	auto onPasteStringSpr = gd::ButtonSprite::create("Paste\nString", 0x28, true, "bigFont.fnt", "GJ_button_04.png", 30.f, .6f);
	auto onPasteString = gd::CCMenuItemSpriteExtra::create(onPasteStringSpr, nullptr, self, menu_selector(EditorPauseLayer::Callback::onPasteString));
	onPasteString->setPosition(actionsMenu->convertToNodeSpace({ winSize.width - 45.f, winSize.height - 30.f }));

	actionsMenu->addChild(onPasteString);

	auto onSettingsSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
	onSettingsSpr->setScale(.65f);
	auto onSettings = gd::CCMenuItemSpriteExtra::create(onSettingsSpr, nullptr, self, menu_selector(gd::MenuLayer::onOptions));
	onSettings->setPosition(actionsMenu->convertToNodeSpace({ winSize.width - 32.f, winSize.height - 66.f }));

	actionsMenu->addChild(onSettings);
}

void __fastcall EditorPauseLayer::dtorH(gd::EditorPauseLayer* self) {
	m_editorPauseLayer = nullptr;
	EditorPauseLayer::dtor(self);
}

void EditorUI::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x5d3e0), EditorUI::initH, reinterpret_cast<void**>(&EditorUI::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x6cf60), EditorUI::createMoveMenuH, reinterpret_cast<void**>(&EditorUI::createMoveMenu));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x6def0), EditorUI::moveForCommandH, reinterpret_cast<void**>(&EditorUI::moveForCommand));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x6e810), EditorUI::transformObjectH, reinterpret_cast<void**>(&EditorUI::transformObject));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x6bad0), EditorUI::onCopyH, reinterpret_cast<void**>(&EditorUI::onCopy));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x5d170), EditorUI::dtorH, reinterpret_cast<void**>(&EditorUI::dtor));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x72960), EditorUI::scrollWheelH, reinterpret_cast<void**>(&EditorUI::scrollWheel));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x71ad0), EditorUI::keyDownH, reinterpret_cast<void**>(&EditorUI::keyDown));
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x72910), EditorUI::keyUpH, reinterpret_cast<void**>(&EditorUI::keyUp));
}

void EditorPauseLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x5aa90), EditorPauseLayer::customSetupH, reinterpret_cast<void**>(&EditorPauseLayer::customSetup));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x5a5f0), EditorPauseLayer::dtorH, reinterpret_cast<void**>(&EditorPauseLayer::dtor));
}