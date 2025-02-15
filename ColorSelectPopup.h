#pragma once
#include "pch.h"

namespace ColorSelectPopup {
	inline bool(__thiscall* init)(gd::ColorSelectPopup*, gd::GameObject*, CCArray*, void*);
	bool __fastcall initH(gd::ColorSelectPopup*, void*, gd::GameObject*, CCArray*, void*);

	inline void(__thiscall* colorValueChanged)(gd::ColorSelectPopup*, ccColor3B);
	void __fastcall colorValueChangedH(gd::ColorSelectPopup*, void*, ccColor3B);

	inline void(__thiscall* closeColorSelect)(gd::ColorSelectPopup*, CCObject*);
	void __fastcall closeColorSelectH(gd::ColorSelectPopup*, void*, CCObject*);

	inline void(__thiscall* updateCopyColorTextInputLabel)(gd::ColorSelectPopup*);
	void __fastcall updateCopyColorTextInputLabelH(gd::ColorSelectPopup*);

	void mem_init();
}