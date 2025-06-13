#pragma once
#include "pch.h"

namespace gd {
	namespace GameToolbox {
		static void transformColor(cocos2d::ccColor3B const& src, cocos2d::ccColor3B& dest, cocos2d::ccHSVValue hsv) {
			reinterpret_cast<void(__thiscall*)(cocos2d::ccColor3B const*, cocos2d::ccColor3B*, cocos2d::ccHSVValue)>(base + 0x22e60)(&src, &dest, hsv);

			__asm add esp, 0x14
		}
	}
}

namespace HSVWidgetPopup {
	inline bool(__thiscall* init)(gd::HSVWidgetPopup*, gd::HSVWidgetPopupDelegate*, ccHSVValue, std::string);
	bool __fastcall initH(gd::HSVWidgetPopup*, void*, gd::HSVWidgetPopupDelegate*, ccHSVValue, std::string);

	inline void(__thiscall* dtor)(gd::HSVWidgetPopup*);
	void __fastcall dtorH(gd::HSVWidgetPopup*);

	class Callback : public gd::HSVWidgetPopup {
	public:
		
	};

	void mem_init();
}

namespace ConfigureHSVWidget {
	inline void(__thiscall* updateLabels)(gd::ConfigureHSVWidget*);
	void __fastcall updateLabelsH(gd::ConfigureHSVWidget*);
}