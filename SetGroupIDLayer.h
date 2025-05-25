#pragma once
#include "pch.h"

namespace SetGroupIDLayer {
	inline bool(__thiscall* init)(gd::SetGroupIDLayer*, gd::GameObject*, CCArray*);
	bool __fastcall initH(gd::SetGroupIDLayer*, void*, gd::GameObject*, CCArray*);

	void mem_init();
}