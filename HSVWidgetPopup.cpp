#include "HSVWidgetPopup.h"
#include "CustomizeObjectLayer.h"

int g_hsvColorChannel = 0; // HJFod
gd::HSVWidgetPopup* m_hsvWidgetPopup;

ccColor3B getColorChannelBaseColor(int channelID, int stackLimit = 0) {
	auto col = gd::GameManager::sharedState()->getLevelEditorLayer()->m_levelSettings->m_effectManager->getColorAction(channelID);
	std::cout << channelID << std::endl;

	if (stackLimit > 10 || !col) return { 255, 255, 255 };

	if (col->m_copyID) {
		auto copy = getColorChannelBaseColor(col->m_copyID, stackLimit + 1);

		gd::GameToolbox::transformColor(copy, copy, col->m_copyHSV);

		return copy;
	}

	return col->m_color;
}

void updateHSVPreviewColor(gd::HSVWidgetPopup* self) {
	auto prev = static_cast<CCSprite*>(self->m_pLayer->getChildByTag(9001));

	if (prev) {
		auto col = getColorChannelBaseColor(reinterpret_cast<int>(prev->getUserData()));

		std::cout << col.r << std::endl;
		std::cout << col.g << std::endl;
		std::cout << col.b << std::endl;

		ccColor3B col3b;
		gd::GameToolbox::transformColor(col, col3b, self->m_configureWidget->m_value);

		std::cout << col3b.r << std::endl;
		std::cout << col3b.g << std::endl;
		std::cout << col3b.b << std::endl;

		prev->setColor(col3b);
	}
}

void __fastcall CustomizeObjectLayer::onHSVH(gd::CustomizeObjectLayer* self, void*, CCObject* sender) {
	g_hsvColorChannel = self->getActiveMode(true);

	CustomizeObjectLayer::onHSV(self, sender);
}

bool __fastcall HSVWidgetPopup::initH(gd::HSVWidgetPopup* self, void*, gd::HSVWidgetPopupDelegate* delegate, ccHSVValue hsv, std::string str) {
	m_hsvWidgetPopup = self;
	if (!HSVWidgetPopup::init(self, delegate, hsv, str)) return false;
	if (g_hsvColorChannel < 1) return true;

	std::cout << m_hsvWidgetPopup << std::endl;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto col = getColorChannelBaseColor(g_hsvColorChannel);

	auto square_p = CCSprite::createWithSpriteFrameName("whiteSquare60_001.png");
	square_p->setPosition({ winSize.width / 2.f - 156.f, winSize.height / 2.f - 15.f });
	square_p->setColor(col);
	square_p->setUserData(reinterpret_cast<void*>(g_hsvColorChannel));
	self->m_pLayer->addChild(square_p, 0, 9000);

	auto square_c = CCSprite::createWithSpriteFrameName("whiteSquare60_001.png");
	square_c->setPosition({ winSize.width / 2.f - 156.f, winSize.height / 2.f + 15.f });
	square_c->setColor(col);
	square_c->setUserData(reinterpret_cast<void*>(g_hsvColorChannel));
	self->m_pLayer->addChild(square_c, 0, 9001);

	updateHSVPreviewColor(self);

	g_hsvColorChannel = 0;

	return true;
}

void __fastcall ConfigureHSVWidget::updateLabelsH(gd::ConfigureHSVWidget* self) {
	ConfigureHSVWidget::updateLabels(self);
	std::cout << m_hsvWidgetPopup << std::endl;
	if (m_hsvWidgetPopup) updateHSVPreviewColor(m_hsvWidgetPopup);
}

void __fastcall HSVWidgetPopup::dtorH(gd::HSVWidgetPopup* self) {
	m_hsvWidgetPopup = nullptr;
	std::cout << "HSVWidgetPopup::dtor\n";
	HSVWidgetPopup::dtor(self);
}

void HSVWidgetPopup::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3ddf0), HSVWidgetPopup::initH, reinterpret_cast<void**>(&HSVWidgetPopup::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3dc50), HSVWidgetPopup::dtorH, reinterpret_cast<void**>(&HSVWidgetPopup::dtor));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x3ec70), ConfigureHSVWidget::updateLabelsH, reinterpret_cast<void**>(&ConfigureHSVWidget::updateLabels));
}