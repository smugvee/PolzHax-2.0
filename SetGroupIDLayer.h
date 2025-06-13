#pragma once
#include "pch.h"

namespace SetGroupIDLayer {
	inline bool(__thiscall* init)(gd::SetGroupIDLayer*, gd::GameObject*, CCArray*);
	bool __fastcall initH(gd::SetGroupIDLayer*, void*, gd::GameObject*, CCArray*);

	inline void(__thiscall* textChanged)(gd::SetGroupIDLayer*, gd::CCTextInputNode*);
	void __fastcall textChangedH(gd::SetGroupIDLayer*, void*, gd::CCTextInputNode*);

	void mem_init();
}