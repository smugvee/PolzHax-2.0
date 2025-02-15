#include "ColorSelectPopup.h"
#include "RGBColorInputWidget.hpp"

RGBColorInputWidget* m_colorInputWidget{ nullptr };

bool __fastcall ColorSelectPopup::initH(gd::ColorSelectPopup* self, void*, gd::GameObject* obj, CCArray* objs, void* colorAction) {
	if (!ColorSelectPopup::init(self, obj, objs, colorAction)) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	m_colorInputWidget = RGBColorInputWidget::create(self);
	self->getLayer()->addChild(m_colorInputWidget);

	m_colorInputWidget->setPosition({ winSize.width / 2.f - 155.f, winSize.height / 2.f + 34.f });

	m_colorInputWidget->setVisible(!from<bool>(self, 0x271));

	return true;
}

void __fastcall ColorSelectPopup::colorValueChangedH(gd::ColorSelectPopup* self, void*, ccColor3B color) {
	ColorSelectPopup::colorValueChanged(self, color);
	if (m_colorInputWidget != nullptr)
		m_colorInputWidget->update_labels(true, true);
}

void __fastcall ColorSelectPopup::closeColorSelectH(gd::ColorSelectPopup* self, void*, CCObject* obj) {
	ColorSelectPopup::closeColorSelect(self, obj);
	m_colorInputWidget = nullptr;
}

void __fastcall ColorSelectPopup::updateCopyColorTextInputLabelH(gd::ColorSelectPopup* self) {
	ColorSelectPopup::updateCopyColorTextInputLabel(self);
	if (m_colorInputWidget != nullptr)
		m_colorInputWidget->setVisible(!from<bool>(self, 0x271));
}

void ColorSelectPopup::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x39e40), ColorSelectPopup::initH, reinterpret_cast<void**>(&ColorSelectPopup::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3cf90), ColorSelectPopup::colorValueChangedH, reinterpret_cast<void**>(&ColorSelectPopup::colorValueChanged));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3ce80), ColorSelectPopup::closeColorSelectH, reinterpret_cast<void**>(&ColorSelectPopup::closeColorSelect));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3d910), ColorSelectPopup::updateCopyColorTextInputLabelH, reinterpret_cast<void**>(&ColorSelectPopup::updateCopyColorTextInputLabel));
}