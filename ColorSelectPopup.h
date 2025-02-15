#pragma once
#include "pch.h"

namespace ColorSelectPopup {
	inline bool(__thiscall* init)(gd::ColorSelectPopup*, gd::GameObject*, CCArray*, void*);
	bool __fastcall initH(gd::ColorSelectPopup*, void*, gd::GameObject*, CCArray*, void*);

	void mem_init();
}