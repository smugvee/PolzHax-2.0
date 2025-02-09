#include "PlayLayer.h"

#include "state.h"

bool __fastcall PlayLayer::initH(gd::PlayLayer* self, void*, gd::GJGameLevel* level) {
	if (!PlayLayer::init(self, level)) return false;
	
	std::cout << gd::GameManager::sharedState() << std::endl;

	if (setting().onHideAttempts)
		self->m_attemptLabel->setVisible(0);
	else
		self->m_attemptLabel->setVisible(1);

	if (gd::GameManager::sharedState()->getGameVariable("0024")) {
		if (setting().onHidePauseButton) {
			from<gd::CCMenuItemSpriteExtra*>(self->m_uiLayer, 0x1a0)->setVisible(0);
		}
		else {
			from<gd::CCMenuItemSpriteExtra*>(self->m_uiLayer, 0x1a0)->setVisible(1);
		}
	}

	if (self->m_isPracticeMode) {
		if (setting().onHidePracticeButtons && (self->m_uiLayer->m_checkpointMenu != nullptr)) {
			self->m_uiLayer->m_checkpointMenu->setVisible(0);
		}
		else {
			self->m_uiLayer->m_checkpointMenu->setVisible(1);
		}
	}

	return true;
}

void __fastcall PlayLayer::updateH(gd::PlayLayer* self, void*, float dt) {
	layers().PauseLayerObject = nullptr;
	PlayLayer::update(self, dt);

	if (setting().onHideAttempts)
		self->m_attemptLabel->setVisible(0);
	else
		self->m_attemptLabel->setVisible(1);

	if (gd::GameManager::sharedState()->getGameVariable("0024")) {
		if (setting().onHidePauseButton) {
			from<gd::CCMenuItemSpriteExtra*>(self->m_uiLayer, 0x1a0)->setVisible(0);
		}
		else {
			from<gd::CCMenuItemSpriteExtra*>(self->m_uiLayer, 0x1a0)->setVisible(1);
		}
	}

	if (self->m_isPracticeMode) {
		if (setting().onHidePracticeButtons && (from<CCMenu*>(self->m_uiLayer, 0x19c) != nullptr)) {
			from<CCMenu*>(self->m_uiLayer, 0x19c)->setVisible(0);
		}
		else {
			from<CCMenu*>(self->m_uiLayer, 0x19c)->setVisible(1);
		}
	}
}

void __fastcall PlayLayer::destroyPlayerH(gd::PlayLayer* self, void*, gd::PlayerObject* player) {
	if (!setting().onNoclip) return PlayLayer::destroyPlayer(self, player);
}

void PlayLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x16ab80), PlayLayer::initH, reinterpret_cast<void**>(&PlayLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x170f30), PlayLayer::updateH, reinterpret_cast<void**>(&PlayLayer::update));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x17b560), PlayLayer::destroyPlayerH, reinterpret_cast<void**>(&PlayLayer::destroyPlayer));
}