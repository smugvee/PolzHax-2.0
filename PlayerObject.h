#pragma once
#include "pch.h"

namespace PlayerObject {
	inline void(__thiscall* updatePlayerFrame)(gd::PlayerObject*, int);
	void __fastcall updatePlayerFrameH(gd::PlayerObject*, void*, int);

	inline void(__thiscall* updatePlayerRollFrame)(gd::PlayerObject*, int);
	void __fastcall updatePlayerRollFrameH(gd::PlayerObject*, void*, int);

	inline void(__thiscall* runBallRotation2)(gd::PlayerObject*);
	void __fastcall runBallRotation2H(gd::PlayerObject*);

	inline void(__thiscall* toggleDartMode)(gd::PlayerObject*, bool);
	void __fastcall toggleDartModeH(gd::PlayerObject*, void*, bool);

	void mem_init();
}