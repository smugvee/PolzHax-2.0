#pragma once
#include "pch.h"

namespace PlayLayer {
	inline bool(__thiscall* init)(gd::PlayLayer*, gd::GJGameLevel*);
	bool __fastcall initH(gd::PlayLayer*, void*, gd::GJGameLevel*);

	inline void(__thiscall* update)(gd::PlayLayer*, float);
	void __fastcall updateH(gd::PlayLayer*, void*, float);

	inline void(__thiscall* destroyPlayer)(gd::PlayLayer*, gd::PlayerObject*);
	void __fastcall destroyPlayerH(gd::PlayLayer*, void*, gd::PlayerObject*);

	void mem_init();
}