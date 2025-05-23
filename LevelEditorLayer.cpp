#include "LevelEditorLayer.h"

#include "Hitboxes.h"

#include "state.h"

bool __fastcall LevelEditorLayer::initH(gd::LevelEditorLayer* self, void*, gd::GJGameLevel* level) {
	if (!LevelEditorLayer::init(self, level)) return false;

	auto playerDrawNode = CCDrawNode::create();
	self->m_objectLayer->addChild(playerDrawNode, 1000, 124);
	auto objectDrawNode = CCDrawNode::create();
	self->m_objectLayer->addChild(objectDrawNode, 1000, 125);

	return true;
}

void __fastcall LevelEditorLayer::updateVisibilityH(gd::LevelEditorLayer* self, void*, float dt) {
	LevelEditorLayer::updateVisibility(self, dt);

	auto playerDrawNode = static_cast<CCDrawNode*>(self->m_objectLayer->getChildByTag(124));
	playerDrawNode->clear();
	auto objectDrawNode = static_cast<CCDrawNode*>(self->m_objectLayer->getChildByTag(125));
	objectDrawNode->clear();

	if (setting().onHitboxes) {
		if (setting().onEnablePlayerHitboxes) {
			if (self->m_player) Hitboxes::drawPlayerHitbox(self->m_player, playerDrawNode);
			if (self->m_player2) Hitboxes::drawPlayerHitbox(self->m_player2, playerDrawNode);
		}

		for (int i = self->m_firstVisibleSection + 1; i <= self->m_lastVisibleSection - 1; i++) {
			if (i < 0) continue;
			if (i >= self->m_levelSections->count()) break;

			auto objectAtIndex = self->m_levelSections->objectAtIndex(i);
			auto objArr = reinterpret_cast<CCArray*>(objectAtIndex);

			for (int j = 0; j < objArr->count(); j++) {
				auto obj = reinterpret_cast<gd::GameObject*>(objArr->objectAtIndex(j));
				if (setting().onEnableSolidHitboxes)
					Hitboxes::drawSolidsObjectHitbox(obj, objectDrawNode);
				if (setting().onEnableHazardHitboxes)
					Hitboxes::drawHazardsObjectHitbox(obj, objectDrawNode);
				if (setting().onEnableSpecialHitboxes)
					Hitboxes::drawSpecialsObjectHitbox(obj, objectDrawNode);
			}
		}
	}
}

void __fastcall LevelEditorLayer::updateH(gd::LevelEditorLayer* self, void*, float dt) {
	LevelEditorLayer::update(self, dt);

	auto playerDrawNode = static_cast<CCDrawNode*>(self->m_objectLayer->getChildByTag(124));
	playerDrawNode->clear();

	if (setting().onHitboxes) {
		if (setting().onEnablePlayerHitboxes) {
			if (self->m_player) Hitboxes::drawPlayerHitbox(self->m_player, playerDrawNode);
			if (self->m_player2) Hitboxes::drawPlayerHitbox(self->m_player2, playerDrawNode);
		}
	}
}

void __fastcall DrawGridLayer::drawH(gd::DrawGridLayer* self) {
	DrawGridLayer::draw(self);

	
}

void LevelEditorLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xed990), LevelEditorLayer::initH, reinterpret_cast<void**>(&LevelEditorLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xf18b0), LevelEditorLayer::updateVisibilityH, reinterpret_cast<void**>(&LevelEditorLayer::updateVisibility));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0xfb840), LevelEditorLayer::updateH, reinterpret_cast<void**>(&LevelEditorLayer::update));
}