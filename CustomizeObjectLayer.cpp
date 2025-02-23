#include "CustomizeObjectLayer.h"
#include "utils.hpp"

bool __fastcall CustomizeObjectLayer::initH(gd::CustomizeObjectLayer* self, void*, gd::GameObject* obj, CCArray* objarr) {
	if (!CustomizeObjectLayer::init(self, obj, objarr)) return false;
	
	auto array = from<CCArray*>(self, 0x1e0);
	auto array2 = from<CCArray*>(self, 0x1e4);
	auto array3 = from<CCArray*>(self, 0x1e8);
	auto array4 = from<CCArray*>(self, 0x1ec);
	auto array5 = from<CCArray*>(self, 0x1f0);

	auto colorSprite_bg = gd::ButtonSprite::create("BG", 0x32, true, "bigFont.fnt", "GJ_button_04.png", 24.f, 1.f);
	auto colorButton_bg = gd::CCMenuItemSpriteExtra::create(colorSprite_bg, nullptr, self, (SEL_MenuHandler)&gd::CustomizeObjectLayer::onSelectColor);
	colorSprite_bg->setTag(1000);
	colorButton_bg->setTag(1000);
	colorButton_bg->setPosition({ -200.f, 194.f });

	array->addObject(colorSprite_bg);
	array2->addObject(colorButton_bg);
	self->getMenu()->addChild(colorButton_bg);

	std::cout << array->count() << std::endl;
	std::cout << array2->count() << std::endl;
	std::cout << array3->count() << std::endl;
	std::cout << array4->count() << std::endl;
	std::cout << array5->count() << std::endl;

	return true;
}

void CustomizeObjectLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x41c60), CustomizeObjectLayer::initH, reinterpret_cast<void**>(&CustomizeObjectLayer::init));
}