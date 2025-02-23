#pragma once
#include "pch.h"

namespace CustomizeObjectLayer {
	inline bool(__thiscall* init)(gd::CustomizeObjectLayer*, gd::GameObject*, CCArray*);
	bool __fastcall initH(gd::CustomizeObjectLayer*, void*, gd::GameObject*, CCArray*);

	void mem_init();
}