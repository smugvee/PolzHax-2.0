#include "CustomizeObjectLayer.h"
#include "utils.hpp"

bool __fastcall CustomizeObjectLayer::initH(gd::CustomizeObjectLayer* self, void*, gd::GameObject* obj, CCArray* objarr) {
	if (!CustomizeObjectLayer::init(self, obj, objarr)) return false;
	
	CCMenu* menu = from<CCMenu*>(self, 0x194);

	auto bg_spr = gd::ButtonSprite::create("BG", 30, true, "bigFont.fnt", "GJ_button_04.png", 25.f, .4f);
	auto bg = gd::CCMenuItemSpriteExtra::create(bg_spr, bg_spr, self, menu_selector(gd::CustomizeObjectLayer::onSelectColor));
	bg_spr->setTag(1000);
	bg->setTag(1000);
	bg->setPosition({ -210.f, 215.f });
	self->m_colorButtons->addObject(bg_spr);
	self->m_colorNodes->addObject(bg);
	self->m_pButtonMenu->addChild(bg);

	auto g_spr = gd::ButtonSprite::create("G1", 30, true, "bigFont.fnt", "GJ_button_04.png", 25.f, .4f);
	auto g = gd::CCMenuItemSpriteExtra::create(g_spr, g_spr, self, menu_selector(gd::CustomizeObjectLayer::onSelectColor));
	g_spr->setTag(1001);
	g->setTag(1001);
	g->setPosition({ -210.f, 185.f });
	self->m_colorButtons->addObject(g_spr);
	self->m_colorNodes->addObject(g);
	self->m_pButtonMenu->addChild(g);

	auto g2_spr = gd::ButtonSprite::create("G2", 30, true, "bigFont.fnt", "GJ_button_04.png", 25.f, .4f);
	auto g2 = gd::CCMenuItemSpriteExtra::create(g2_spr, g2_spr, self, menu_selector(gd::CustomizeObjectLayer::onSelectColor));
	g2_spr->setTag(1009);
	g2->setTag(1009);
	g2->setPosition({ -210.f, 155.f });
	self->m_colorButtons->addObject(g2_spr);
	self->m_colorNodes->addObject(g2);
	self->m_pButtonMenu->addChild(g2);

	auto l_spr = gd::ButtonSprite::create("Line", 30, true, "bigFont.fnt", "GJ_button_04.png", 25.f, .4f);
	auto l = gd::CCMenuItemSpriteExtra::create(l_spr, l_spr, self, menu_selector(gd::CustomizeObjectLayer::onSelectColor));
	l_spr->setTag(1002);
	l->setTag(1002);
	l->setPosition({ -210.f, 125.f });
	self->m_colorButtons->addObject(l_spr);
	self->m_colorNodes->addObject(l);
	self->m_pButtonMenu->addChild(l);

	auto obj_spr = gd::ButtonSprite::create("Obj", 30, true, "bigFont.fnt", "GJ_button_04.png", 25.f, .4f);
	auto obj_ = gd::CCMenuItemSpriteExtra::create(obj_spr, obj_spr, self, menu_selector(gd::CustomizeObjectLayer::onSelectColor));
	obj_spr->setTag(1004);
	obj_->setTag(1004);
	obj_->setPosition({ -210.f, 95.f });
	self->m_colorButtons->addObject(obj_spr);
	self->m_colorNodes->addObject(obj_);
	self->m_pButtonMenu->addChild(obj_);

	auto dl_spr = gd::ButtonSprite::create("3DL", 30, true, "bigFont.fnt", "GJ_button_04.png", 25.f, .4f);
	auto dl = gd::CCMenuItemSpriteExtra::create(dl_spr, dl_spr, self, menu_selector(gd::CustomizeObjectLayer::onSelectColor));
	dl_spr->setTag(1003);
	dl->setTag(1003);
	dl->setPosition({ -210.f, 65.f });
	self->m_colorButtons->addObject(dl_spr);
	self->m_colorNodes->addObject(dl);
	self->m_pButtonMenu->addChild(dl);

	auto black_spr = gd::ButtonSprite::create("Black", 30, true, "bigFont.fnt", "GJ_button_04.png", 25.f, .4f);
	auto black = gd::CCMenuItemSpriteExtra::create(black_spr, black_spr, self, menu_selector(gd::CustomizeObjectLayer::onSelectColor));
	black_spr->setTag(1010);
	black->setTag(1010);
	black->setPosition({ -210.f, 35.f });
	self->m_colorButtons->addObject(black_spr);
	self->m_colorNodes->addObject(black);
	self->m_pButtonMenu->addChild(black);

	auto white_spr = gd::ButtonSprite::create("White", 30, true, "bigFont.fnt", "GJ_button_04.png", 25.f, .4f);
	auto white = gd::CCMenuItemSpriteExtra::create(white_spr, white_spr, self, menu_selector(gd::CustomizeObjectLayer::onSelectColor));
	white_spr->setTag(1011);
	white->setTag(1011);
	white->setPosition({ -210.f, 5.f });
	self->m_colorButtons->addObject(white_spr);
	self->m_colorNodes->addObject(white);
	self->m_pButtonMenu->addChild(white);

	auto lighter_spr = gd::ButtonSprite::create("Lighter", 30, true, "bigFont.fnt", "GJ_button_04.png", 25.f, .4f);
	auto lighter = gd::CCMenuItemSpriteExtra::create(lighter_spr, lighter_spr, self, menu_selector(gd::CustomizeObjectLayer::onSelectColor));
	lighter_spr->setTag(1011);
	lighter->setTag(1012);
	lighter->setPosition({ -210.f, -25.f });
	self->m_colorButtons->addObject(lighter_spr);
	self->m_colorNodes->addObject(lighter);
	self->m_pButtonMenu->addChild(lighter);

	return true;
}

void CustomizeObjectLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x41c60), CustomizeObjectLayer::initH, reinterpret_cast<void**>(&CustomizeObjectLayer::init));
}