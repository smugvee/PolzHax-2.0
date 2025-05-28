#include "PlayLayer.h"

#include "state.h"
#include "Checkpoint.h"
#include <vector>
#include "utils.hpp"

#include "Hitboxes.h"

#include "imgui.h"

std::vector<CheckPoint> checkpoints;

std::vector<gd::StartPosObject*> m_startPositions;
std::vector<gd::GameObject*> m_dualPortals, m_gamemodePortals, m_miniPortals, m_speedChanges, m_mirrorPortals;

CCArray* startPosArray;
int currentStartPos = 0;
bool fadeOutFlag = false;
CCLabelBMFont* startPosSwitcherLabel = nullptr;
std::vector<gd::StartPosObject*> startPosObjects;

bool inPractice;
bool inTestmode;
int smoothOut;

static gd::GameObject* getClosestObject(std::vector<gd::GameObject*>& vec, gd::StartPosObject* startPos) {
	gd::GameObject* closest = nullptr;

	std::ranges::sort(vec, [](gd::GameObject* a, gd::GameObject* b) {
		return a->getPositionX() < b->getPositionX();
		});

	for (auto obj : vec) {
		if (obj->getPositionX() - 10 > startPos->getPositionX())
			break;
		if (obj->getPositionX() - 10 < startPos->getPositionX())
			closest = obj;
	}

	return closest;
}

void setupStartPos(gd::StartPosObject* startPos) { // Eclipse Menu
	gd::LevelSettingsObject* startPosSettings = startPos->m_levelSettings;
	gd::LevelSettingsObject* levelSettings = gd::GameManager::sharedState()->getPlayLayer()->m_levelSettings;

	startPosSettings->m_startDual = levelSettings->m_startDual;
	startPosSettings->m_startMode = levelSettings->m_startMode;
	startPosSettings->m_startMini = levelSettings->m_startMini;
	startPosSettings->m_startSpeed = levelSettings->m_startSpeed;

	gd::GameObject* obj = getClosestObject(m_dualPortals, startPos);
	if (obj)
		startPosSettings->m_startDual = obj->m_objectID == 286;

	obj = getClosestObject(m_gamemodePortals, startPos);

	if (obj) {
		switch (obj->m_objectID) {
		case 12: startPosSettings->m_startMode = 0;
			break;
		case 13: startPosSettings->m_startMode = 1;
			break;
		case 47: startPosSettings->m_startMode = 2;
			break;
		case 111: startPosSettings->m_startMode = 3;
			break;
		case 660: startPosSettings->m_startMode = 4;
			break;
		case 745: startPosSettings->m_startMode = 5;
			break;
		default: break;
		}
	}

	obj = getClosestObject(m_miniPortals, startPos);

	if (obj)
		startPosSettings->m_startMini = obj->m_objectID == 101;

	obj = getClosestObject(m_speedChanges, startPos);
	if (obj) {
		switch (obj->m_objectID) {
		case 200: startPosSettings->m_startSpeed = 1;
			break;
		case 201: startPosSettings->m_startSpeed = 0;
			break;
		case 202: startPosSettings->m_startSpeed = 2;
			break;
		case 203: startPosSettings->m_startSpeed = 3;
			break;
		default: break;
		}
	}
}

void pickStartPos(gd::PlayLayer* playLayer, int32_t index) { // Eclipse Menu
	if (startPosObjects.empty()) return;

	auto count = static_cast<int32_t>(startPosObjects.size());
	if (index >= count) index = -1;
	else if (index < -1) index = count - 1;

	currentStartPos = index;
	
	auto* startPos = index >= 0 ? startPosObjects[index] : nullptr;
	if (startPos != playLayer->m_startPosObject) {
		if (startPos) {
			startPos->retain();
		}
		if (playLayer->m_startPosObject) {
			playLayer->m_startPosObject->release();
		}
		playLayer->m_startPosObject = startPos;
	}
	if (currentStartPos >= 0) { // Taswert's thing
		playLayer->m_playerStartPosition = playLayer->m_startPosObject->getOrientedBox()->m_center;
	}
	else {
		playLayer->m_playerStartPosition = ccp(0, 105);
	}
	playLayer->m_isTestMode = index >= 0;

	playLayer->resetLevel();
	playLayer->startMusic();

	auto spSwitcherLabel = static_cast<CCLabelBMFont*>(static_cast<CCMenu*>(playLayer->m_uiLayer->getChildByTag(177))->getChildByTag(178));
	if (spSwitcherLabel) {
		//gd::StartPosObject* prevSP = nullptr;
		//gd::StartPosObject* nextSP = nullptr;

		//if (currentStartPos == -1) {
		//	prevSP = startPosObjects.back();
		//}
		//else if (currentStartPos + 1 == startPosObjects.size()) {
		//	nextSP = startPosObjects.front();
		//}
		//else {
		//	prevSP = index >= 0 ? startPosObjects[currentStartPos - 1] : nullptr;
		//	nextSP = index >= 0 ? startPosObjects[currentStartPos + 1] : nullptr;
		//}
		//if (nextSP && prevSP) {
		//	std::cout << "Previous StartPosObject: " << prevSP << std::endl;
		//	std::cout << prevSP->getPositionX() / playLayer->m_levelLength * 100.f << "%" << std::endl;
		//	std::cout << "Next StartPosObject: " << nextSP << std::endl;
		//	std::cout << nextSP->getPositionX() / playLayer->m_levelLength * 100.f << "%" << std::endl;
		//}

		spSwitcherLabel->setString(CCString::createWithFormat("%d/%d", currentStartPos + 1, startPosObjects.size())->getCString());
		spSwitcherLabel->setOpacity(255);
		spSwitcherLabel->stopAllActions();
		spSwitcherLabel->runAction(CCSequence::create(CCDelayTime::create(1.f), CCFadeOut::create(0.5f), nullptr));
	}
}

void PlayLayer::onNextStartPos() {
	pickStartPos(gd::GameManager::sharedState()->getPlayLayer(), currentStartPos + 1);
}

void PlayLayer::onPrevStartPos() {
	pickStartPos(gd::GameManager::sharedState()->getPlayLayer(), currentStartPos - 1);
}

bool __fastcall PlayLayer::initH(gd::PlayLayer* self, void*, gd::GJGameLevel* level) {
	startPosObjects.clear();
	if (!PlayLayer::init(self, level)) return false;
	inPractice = false;
	inTestmode = self->m_isTestMode;
	smoothOut = 0;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();
	
	std::cout << "GameManager: " << gd::GameManager::sharedState() << std::endl;
	std::cout << "FMODAudioEngine: " << gd::FMODAudioEngine::sharedEngine() << std::endl;

	self->m_attemptLabel->setVisible(!setting().onHideAttempts);

	if (gd::GameManager::sharedState()->getGameVariable("0024")) {
		from<gd::CCMenuItemSpriteExtra*>(self->m_uiLayer, 0x1a0)->setVisible(!setting().onHidePauseButton);
	}

	self->m_player->setVisible(!setting().onHidePlayer);
	self->m_player2->setVisible(!setting().onHidePlayer);

	if (setting().onPracticeBugFix) {
		checkpoints.clear();
	}

	m_dualPortals.clear();
	m_gamemodePortals.clear();
	m_miniPortals.clear();
	m_miniPortals.clear();
	m_speedChanges.clear();
	m_mirrorPortals.clear();
	m_startPositions.clear();

	if (setting().onAutoSafeMode && setting().cheatsCount > 0) safeModeON(), setting().isSafeMode = true;
	else if (!setting().onSafeMode) safeModeOFF(), setting().isSafeMode = false;

	const auto bar = gd::GameManager::sharedState()->getProgressBar();
	auto size = CCDirector::sharedDirector()->getWinSize();
	auto percentLabel = from<CCLabelBMFont*>(self, 0x25c);
	if (percentLabel) {
		if (bar) {
			percentLabel->setPosition({ size.width / 2.f + 110.2f, size.height - 8.f });
			percentLabel->setAnchorPoint({ 0.f, 0.5f });
		}
		else {
			percentLabel->setPosition({ size.width / 2.f, size.height - 8.f });
			percentLabel->setAnchorPoint({ 0.5f, 0.5f });
		}
		percentLabel->setVisible(gd::GameManager::sharedState()->getGameVariable("0040"));
	}

	auto spSwitcherMenu = CCMenu::create();
	self->m_uiLayer->addChild(spSwitcherMenu, 16, 177);
	auto spSwitcherLabel = CCLabelBMFont::create("", "bigFont.fnt");
	spSwitcherMenu->addChild(spSwitcherLabel, 0, 178);
	spSwitcherLabel->setPositionY(-145.f);
	spSwitcherLabel->setScale(.5f);
	if (!startPosObjects.empty()) spSwitcherLabel->setString(CCString::createWithFormat("%d/%d", currentStartPos + 1, startPosObjects.size())->getCString());
	spSwitcherLabel->setOpacity(255);
	spSwitcherLabel->stopAllActions();
	spSwitcherLabel->runAction(CCSequence::create(CCDelayTime::create(1.f), CCFadeOut::create(0.5f), nullptr));

	auto playerDrawNode = CCDrawNode::create();
	self->m_objectLayer->addChild(playerDrawNode, 1000, 124);
	auto objectDrawNode = CCDrawNode::create();
	self->m_objectLayer->addChild(objectDrawNode, 1000, 125);

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

	return true;
}

void __fastcall PlayLayer::updateH(gd::PlayLayer* self, void*, float dt) {
	layers().PauseLayerObject = nullptr;
	PlayLayer::update(self, dt);

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	self->m_attemptLabel->setVisible(!setting().onHideAttempts);

	if (gd::GameManager::sharedState()->getGameVariable("0024")) {
		from<gd::CCMenuItemSpriteExtra*>(self->m_uiLayer, 0x1a0)->setVisible(!setting().onHidePauseButton);
	}

	self->m_player->setVisible(!setting().onHidePlayer);
	self->m_player2->setVisible(!setting().onHidePlayer);

	if (setting().onAutoSafeMode && setting().cheatsCount > 0) safeModeON(), setting().isSafeMode = true;
	else if (!setting().onSafeMode) safeModeOFF(), setting().isSafeMode = false;

	if (setting().onLockCursor && !setting().show && !self->m_hasLevelCompletedMenu && !self->m_isDead) {
		SetCursorPos(winSize.width / 2, winSize.height / 2);
	}

	auto playerDrawNode = static_cast<CCDrawNode*>(self->m_objectLayer->getChildByTag(124));
	playerDrawNode->clear();
	auto objectDrawNode = static_cast<CCDrawNode*>(self->m_objectLayer->getChildByTag(125));
	objectDrawNode->clear();

	if ((self->m_player->m_isDead && setting().onHitboxesOnDeath) || setting().onHitboxes) {
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

void __fastcall PlayLayer::destroyPlayerH(gd::PlayLayer* self, void*, gd::PlayerObject* player) {
	if (!setting().onNoclip) return PlayLayer::destroyPlayer(self, player);
}

void __fastcall PlayLayer::resetLevelH(gd::PlayLayer* self) {
	if (setting().onSmartStartPos)
		for (gd::StartPosObject* obj : m_startPositions)
			setupStartPos(obj);

	PlayLayer::resetLevel(self);

	if (setting().onPracticeBugFix) {
		if (self->m_isPracticeMode && checkpoints.size() > 0)
			checkpoints.back().restore(self);
	}
}

void __fastcall PlayLayer::togglePracticeModeH(gd::PlayLayer* self, void* edx, bool practice) {
	if (setting().onPracticeBugFix) {
		checkpoints.clear();
	}
	PlayLayer::togglePracticeMode(self, practice);

	if (setting().onHidePracticeButtons)
		self->m_uiLayer->m_checkpointMenu->setVisible(!setting().onHidePracticeButtons);
}

void __fastcall PlayLayer::createCheckpointH(gd::PlayLayer* self) {
	if (setting().onPracticeBugFix) {
		if (self->m_player != nullptr) {
			checkpoints.push_back({
				CheckPoint::from(self)
				});
		}
	}
	PlayLayer::createCheckpoint(self);
}

void __fastcall PlayLayer::removeLastCheckpointH(gd::PlayLayer* self) {
	if (setting().onPracticeBugFix) {
		if (checkpoints.size() > 0) {
			checkpoints.pop_back();
		}
	}
	PlayLayer::removeLastCheckpoint(self);
}

void __fastcall PlayLayer::onQuitH(gd::PlayLayer* self) {
	if (setting().onPracticeBugFix) {
		checkpoints.clear();
	}
	PlayLayer::onQuit(self);
}

void __fastcall PlayLayer::levelCompleteH(gd::PlayLayer* self) {
	if (setting().onPracticeBugFix) {
		checkpoints.clear();
	}
	PlayLayer::levelComplete(self);
}

void __fastcall PlayLayer::addObjectH(gd::PlayLayer* self, void*, gd::GameObject* obj) {
	PlayLayer::addObject(self, obj);

	if (obj->m_objectID == 31) {
		startPosObjects.push_back(reinterpret_cast<gd::StartPosObject*>(obj));
		std::cout << startPosObjects.size() << std::endl;
	}

	switch (obj->m_objectID)
	{
	case 31:
		m_startPositions.push_back(static_cast<gd::StartPosObject*>(obj));
		break;
	case 12:
	case 13:
	case 47:
	case 111:
	case 660:
	case 745: m_gamemodePortals.push_back(obj);
		break;
	case 45:
	case 46: m_mirrorPortals.push_back(obj);
		break;
	case 99:
	case 101: m_miniPortals.push_back(obj);
		break;
	case 286:
	case 287: m_dualPortals.push_back(obj);
		break;
	case 200:
	case 201:
	case 202:
	case 203: m_speedChanges.push_back(obj);
		break;
	default: break;
	}
}

void __fastcall PlayLayer::createObjectsFromSetupH(gd::PlayLayer* self, void*, gd::string objects) {
	PlayLayer::createObjectsFromSetup(self, objects);
	if (startPosObjects.empty()) return;

	std::ranges::sort(startPosObjects, [](gd::GameObject* a, gd::GameObject* b) {
		return a->getPositionX() < b->getPositionX();
		});

	currentStartPos = -1;
	if (self->m_startPosObject) {
		auto it = std::ranges::find(startPosObjects, self->m_startPosObject);
		if (it != startPosObjects.end())
			currentStartPos = static_cast<int32_t>(std::distance(startPosObjects.begin(), it));
	}
}

void __fastcall PlayLayer::lightningFlashH(gd::PlayLayer* self, void*, CCPoint p0, ccColor3B p1) {
	PlayLayer::lightningFlash(self, p0, p1);
	//self->stopActionByTag(1);
}

void __fastcall PlayLayer::updateVisibilityH(gd::PlayLayer* self) {
	PlayLayer::updateVisibility(self);
	if (setting().onNoWavePulse) {
		self->m_player->m_audioScale = 1.f;
		self->m_player2->m_audioScale = 1.f;
	}
}

void PlayLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x16ab80), PlayLayer::initH, reinterpret_cast<void**>(&PlayLayer::init));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x170f30), PlayLayer::updateH, reinterpret_cast<void**>(&PlayLayer::update));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x17b560), PlayLayer::destroyPlayerH, reinterpret_cast<void**>(&PlayLayer::destroyPlayer));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x17c930), PlayLayer::resetLevelH, reinterpret_cast<void**>(&PlayLayer::resetLevel));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x17d900), PlayLayer::togglePracticeModeH, reinterpret_cast<void**>(&PlayLayer::togglePracticeMode));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x17c0e0), PlayLayer::createCheckpointH, reinterpret_cast<void**>(&PlayLayer::createCheckpoint));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x17c780), PlayLayer::removeLastCheckpointH, reinterpret_cast<void**>(&PlayLayer::removeLastCheckpoint));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x17de80), PlayLayer::onQuitH, reinterpret_cast<void**>(&PlayLayer::onQuit));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x16c830), PlayLayer::levelCompleteH, reinterpret_cast<void**>(&PlayLayer::levelComplete));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x16fc00), PlayLayer::addObjectH, reinterpret_cast<void**>(&PlayLayer::addObject));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x16f700), PlayLayer::createObjectsFromSetupH, reinterpret_cast<void**>(&PlayLayer::createObjectsFromSetup));
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x176470), PlayLayer::updateVisibilityH, reinterpret_cast<void**>(&PlayLayer::updateVisibility));
}