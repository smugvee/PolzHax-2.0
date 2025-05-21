#include "InfoLayer.h"

void InfoLayer::Callback::onRefreshComments(CCObject*) {
	auto glm = gd::GameLevelManager::sharedState();
	glm->makeTimeStamp("upd_comments");
	glm->resetTimerForKey(this->m_commentsKey.c_str());
	this->loadPage(this->m_page);
}

bool __fastcall InfoLayer::initH(gd::InfoLayer* self, void*, gd::GJGameLevel* level) {
	if (!InfoLayer::init(self, level)) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto sprite = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
	sprite->setScale(.7f);
	auto button = gd::CCMenuItemSpriteExtra::create(sprite, nullptr, self, menu_selector(InfoLayer::Callback::onRefreshComments));
	button->setPositionX(195.f);

	self->m_pButtonMenu->addChild(button);

	return true;
}