#include "PlayerObject.h"
#include "state.h"

void __fastcall PlayerObject::updatePlayerFrameH(gd::PlayerObject* self, void*, int frameID) {
	if (setting().onNoMiniIcon)
		return PlayerObject::updatePlayerFrame(self, gd::GameManager::sharedState()->m_playerFrame);
	PlayerObject::updatePlayerFrame(self, frameID);
}

void __fastcall PlayerObject::updatePlayerRollFrameH(gd::PlayerObject* self, void*, int frameID) {
	if (setting().onNoMiniIcon)
		return PlayerObject::updatePlayerRollFrame(self, gd::GameManager::sharedState()->m_playerBall);
	PlayerObject::updatePlayerRollFrame(self, frameID);
}

void __fastcall PlayerObject::runBallRotation2H(gd::PlayerObject* self) {
	if (setting().onBallRotatingBugFix) self->stopActionByTag(1);
	PlayerObject::runBallRotation2(self);
}

void __fastcall PlayerObject::toggleDartModeH(gd::PlayerObject* self, void*, bool p0) {
	PlayerObject::toggleDartMode(self, p0);
	if (setting().onNoWaveTrailBehind) from<bool>(self->m_motionStreak, 0xfe) = false;
}

void PlayerObject::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x167400), PlayerObject::updatePlayerFrameH, reinterpret_cast<void**>(&PlayerObject::updatePlayerFrame));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x167840), PlayerObject::updatePlayerRollFrameH, reinterpret_cast<void**>(&PlayerObject::updatePlayerRollFrame));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x160b20), PlayerObject::runBallRotation2H, reinterpret_cast<void**>(&PlayerObject::runBallRotation2));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x165e30), PlayerObject::toggleDartModeH, reinterpret_cast<void**>(&PlayerObject::toggleDartMode));
}