#include "ColorSelectPopup.h"
#include "RGBColorInputWidget.hpp"

bool __fastcall ColorSelectPopup::initH(gd::ColorSelectPopup* self, void*, gd::GameObject* obj, CCArray* objs, void* colorAction) {
	if (!ColorSelectPopup::init(self, obj, objs, colorAction)) return false;

	auto RGBColorInputWidget = RGBColorInputWidget::create(self);
	self->getLayer()->addChild(RGBColorInputWidget);

	return true;
}

void ColorSelectPopup::mem_init() {
	//MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x39e40), ColorSelectPopup::initH, reinterpret_cast<void**>(&ColorSelectPopup::init));
}