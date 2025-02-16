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

bool __fastcall SetupPulsePopup::initH(gd::SetupPulsePopup* self, void*, gd::EffectGameObject* obj, CCArray* objs) {
	if (!SetupPulsePopup::init(self, obj, objs)) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	m_colorInputWidget = RGBColorInputWidget::create(reinterpret_cast<gd::ColorSelectPopup*>(self));
	self->getLayer()->addChild(m_colorInputWidget);

	self->m_colorPicker->setPositionX(self->m_colorPicker->getPositionX() + 3.7f);
	m_colorInputWidget->setPosition({ winSize.width / 2.f - 132.f, winSize.height / 2.f + 32.f });

	auto square_width = from<CCSprite*>(self, 0x208)->getContentSize().width * from<CCSprite*>(self, 0x208)->getScaleX();
	auto x = m_colorInputWidget->getPositionX() - square_width / 2.f;

	from<CCSprite*>(self, 0x208)->setPosition({ x + 20.0f, winSize.height / 2.f + 85.f });
	from<CCSprite*>(self, 0x20c)->setPosition({ x + 20.0f + square_width, winSize.height / 2.f + 85.f });

	m_colorInputWidget->setVisible(from<int>(self, 0x274) == 0);

	return true;
}

void __fastcall SetupPulsePopup::colorValueChangedH(gd::SetupPulsePopup* self) {
	SetupPulsePopup::colorValueChanged(self);
	if (m_colorInputWidget != nullptr)
		m_colorInputWidget->update_labels(true, true);
}

void __fastcall SetupPulsePopup::closeColorSelectH(gd::SetupPulsePopup* self, void*, CCObject* obj) {
	SetupPulsePopup::closeColorSelect(self, obj);
	m_colorInputWidget = nullptr;
}

void __fastcall SetupPulsePopup::updatePulseModeH(gd::SetupPulsePopup* self) {
	SetupPulsePopup::updatePulseMode(self);
	if (m_colorInputWidget != nullptr)
		m_colorInputWidget->setVisible(from<int>(self, 0x274) == 0);
}

void ColorSelectPopup::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x39e40), ColorSelectPopup::initH, reinterpret_cast<void**>(&ColorSelectPopup::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3cf90), ColorSelectPopup::colorValueChangedH, reinterpret_cast<void**>(&ColorSelectPopup::colorValueChanged));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3ce80), ColorSelectPopup::closeColorSelectH, reinterpret_cast<void**>(&ColorSelectPopup::closeColorSelect));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3d910), ColorSelectPopup::updateCopyColorTextInputLabelH, reinterpret_cast<void**>(&ColorSelectPopup::updateCopyColorTextInputLabel));
}

void SetupPulsePopup::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x191e20), SetupPulsePopup::initH, reinterpret_cast<void**>(&SetupPulsePopup::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x1957b0), SetupPulsePopup::closeColorSelectH, reinterpret_cast<void**>(&SetupPulsePopup::closeColorSelect));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x195940), SetupPulsePopup::colorValueChangedH, reinterpret_cast<void**>(&SetupPulsePopup::colorValueChanged));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x195f70), SetupPulsePopup::updatePulseModeH, reinterpret_cast<void**>(&SetupPulsePopup::updatePulseMode));
}