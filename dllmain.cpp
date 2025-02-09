#include "pch.h"

#include "menu.h"
#include "state.h"
#include "utils.hpp"
#include "patching.h"

#include "PlayLayer.h"
#include "PauseLayer.h"
#include "EditorUI.h"

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

void(__thiscall* CCParticleSystemQuad_draw)(CCParticleSystemQuad*);
void __fastcall CCParticleSystemQuad_drawH(CCParticleSystemQuad* self) {
	if (!setting().onNoParticles) return CCParticleSystemQuad_draw(self);
}

void(__thiscall* PlayerObject_updatePlayerFrame)(gd::PlayerObject*, int);
void __fastcall PlayerObject_updatePlayerFrameH(gd::PlayerObject* self, void* edx, int frameID) {
	if (setting().onNoMiniIcon) {
		return PlayerObject_updatePlayerFrame(self, from<int>(gd::GameManager::sharedState(), 0x1c8));
	}
	PlayerObject_updatePlayerFrame(self, frameID);
}

void(__thiscall* PlayerObject_updatePlayerRollFrame)(gd::PlayerObject*, int);
void __fastcall PlayerObject_updatePlayerRollFrameH(gd::PlayerObject* self, void* edx, int frameID) {
	if (setting().onNoMiniIcon) {
		return PlayerObject_updatePlayerRollFrame(self, from<int>(gd::GameManager::sharedState(), 0x1e0));
	}
	PlayerObject_updatePlayerRollFrame(self, frameID);
}

void(__thiscall* PlayerObject_runBallRotation2)(gd::PlayerObject*);
void __fastcall PlayerObject_runBallRotation2H(gd::PlayerObject* self) {
	if (setting().onBallRotatingBugFix) {
		self->stopActionByTag(1);
	}
	PlayerObject_runBallRotation2(self);
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
	setting().loadState();

	sequence_patch((uint32_t)gd::base + 0x558db, { 0xb8, 0x01, 0x00, 0x00, 0x00, 0x90, 0x90 });
	sequence_patch((uint32_t)gd::base + 0x38a85, { 0x6a, 0x00 });

	AllocConsole();
	freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);

	if (MH_Initialize() != MH_OK) {
		FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(hModule), 0);
	}

	auto cocos = GetModuleHandleA("libcocos2d.dll");
	auto cocos_ext = GetModuleHandleA("libExtensions.dll");

	PlayLayer::mem_init();
	PauseLayer::mem_init();
	EditorPauseLayer::mem_init();

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9afc0), GameManager_isIconUnlockedH, reinterpret_cast<void**>(&GameManager_isIconUnlocked));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x9b2a0), GameManager_isColorUnlockedH, reinterpret_cast<void**>(&GameManager_isColorUnlocked));

	MH_CreateHook(reinterpret_cast<void*>(GetProcAddress(cocos, "?draw@CCParticleSystemQuad@cocos2d@@UAEXXZ")), CCParticleSystemQuad_drawH, reinterpret_cast<void**>(&CCParticleSystemQuad_draw));

	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x167400), PlayerObject_updatePlayerFrameH, reinterpret_cast<void**>(&PlayerObject_updatePlayerFrame));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x167840), PlayerObject_updatePlayerRollFrameH, reinterpret_cast<void**>(&PlayerObject_updatePlayerRollFrame));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x160b20), PlayerObject_runBallRotation2H, reinterpret_cast<void**>(&PlayerObject_runBallRotation2));

	MH_CreateHook(
		reinterpret_cast<void*>(gd::base + 0x392a0),
		reinterpret_cast<void**>(&AppDelegate_trySaveGameH),
		reinterpret_cast<void**>(&AppDelegate_trySaveGame));

	setup_imgui_menu();

	MH_EnableHook(MH_ALL_HOOKS);

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
	return TRUE;
}

