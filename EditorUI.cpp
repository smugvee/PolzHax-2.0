#include "EditorUI.h"

class SaveLevelProtocol : public gd::FLAlertLayerProtocol {
protected:
	void FLAlert_Clicked(gd::FLAlertLayer* layer, bool btn2) {
		if (btn2) {
			std::cout << layer->getParent() << std::endl;
		}
	}
};

void EditorPauseLayer::Callback::onSaveLevel(CCObject*) {
	gd::FLAlertLayer::create(nullptr, "test", 200.f, "OK", nullptr, "idk");
}

void __fastcall EditorPauseLayer::customSetupH(gd::EditorPauseLayer* self) {
	EditorPauseLayer::customSetup(self);

	auto menu = CCMenu::create();
	auto button = gd::CCMenuItemSpriteExtra::create(
		CCSprite::create("GJ_Button_01.png"),
		nullptr,
		self,
		menu_selector(EditorPauseLayer::Callback::onSaveLevel)
	);
	menu->addChild(button);
	self->addChild(menu);

	gd::GameManager::sharedState()->setGameVariable("0045", true);
}

void EditorPauseLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x5aa90), EditorPauseLayer::customSetupH, reinterpret_cast<void**>(&EditorPauseLayer::customSetup));
}