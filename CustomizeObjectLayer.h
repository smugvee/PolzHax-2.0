#pragma once
#include "pch.h"

namespace CustomizeObjectLayer {
	inline bool(__thiscall* init)(gd::CustomizeObjectLayer*, gd::GameObject*, CCArray*);
	bool __fastcall initH(gd::CustomizeObjectLayer*, void*, gd::GameObject*, CCArray*);

	inline void(__thiscall* highlightSelected)(gd::CustomizeObjectLayer*, gd::ButtonSprite*);
	void __fastcall highlightSelectedH(gd::CustomizeObjectLayer*, gd::ButtonSprite*);

	void mem_init();
}