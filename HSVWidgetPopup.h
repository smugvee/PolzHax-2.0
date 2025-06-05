#pragma once
#include "pch.h"

namespace gd {
	namespace GameToolbox {
		static void transformColor(cocos2d::ccColor3B const& src, cocos2d::ccColor3B& dest, cocos2d::ccHSVValue hsv) {
			reinterpret_cast<void(__thiscall*)(cocos2d::ccColor3B const*, cocos2d::ccColor3B*, cocos2d::ccHSVValue)>(base + 0x22e60)(&src, &dest, hsv);

			__asm add esp, 0x14
		}
	}

	class ConfigureHSVWidget : public cocos2d::CCNode {
	public:
		cocos2d::CCLabelBMFont* m_hueLabel; // 0xe8
		cocos2d::CCLabelBMFont* m_saturationLabel; // 0xec
		cocos2d::CCLabelBMFont* m_brightnessLabel; // 0xf0
		Slider* m_hueSlider; // 0xf4
		Slider* m_saturationSlider; // 0xf8
		Slider* m_brightnessSlider; // 0xfc
		cocos2d::ccHSVValue m_value; // 0x100
	};

	class HSVWidgetPopup : public FLAlertLayer {
	public:
		ConfigureHSVWidget* m_configureWidget;
		HSVWidgetPopupDelegate* m_delegate;
	};
}

namespace HSVWidgetPopup {
	inline bool(__thiscall* init)(gd::HSVWidgetPopup*, gd::HSVWidgetPopupDelegate*, ccHSVValue, std::string);
	bool __fastcall initH(gd::HSVWidgetPopup*, void*, gd::HSVWidgetPopupDelegate*, ccHSVValue, std::string);

	class Callback : public gd::HSVWidgetPopup {
	public:
		
	};

	void mem_init();
}

namespace ConfigureHSVWidget {
	inline void(__thiscall* updateLabels)(gd::ConfigureHSVWidget*);
	void __fastcall updateLabelsH(gd::ConfigureHSVWidget*);
}