#include "PlayLayer.h"

#include "state.h"
#include "Checkpoint.h"
#include <vector>
#include "utils.hpp"

std::vector<CheckPoint> checkpoints;

std::vector<gd::StartPosObject*> m_startPositions;
std::vector<gd::GameObject*> m_dualPortals, m_gamemodePortals, m_miniPortals, m_speedChanges, m_mirrorPortals;

CCArray* startPosArray;
int currentStartPos = 0;
bool fadeOutFlag = false;
CCLabelBMFont* startPosSwitcherLabel = nullptr;
std::vector<gd::StartPosObject*> startPosObjects;

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

void setupStartPos(gd::StartPosObject* startPos) {
	gd::LevelSettingsObject* startPosSettings = startPos->m_levelSettings;
	gd::LevelSettingsObject* levelSettings = gd::GameManager::sharedState()->getPlayLayer()->m_levelSettings;

	startPosSettings->m_startDual = levelSettings->m_startDual;
	startPosSettings->m_startMode = levelSettings->m_startMode;
	startPosSettings->m_startMini = levelSettings->m_startMini;
	startPosSettings->m_startSpeed = levelSettings->m_startSpeed;

	gd::GameObject* obj = getClosestObject(m_dualPortals, startPos);
	if (obj)
		startPosSettings->m_startDual = from<int>(obj, 0x310) == 286;

	obj = getClosestObject(m_gamemodePortals, startPos);

	if (obj) {
		switch (from<int>(obj, 0x310)) {
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
		startPosSettings->m_startMini = from<int>(obj, 0x310) == 101;

	obj = getClosestObject(m_speedChanges, startPos);
	if (obj) {
		switch (from<int>(obj, 0x310)) {
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

void PlayLayer::onNextStartPos() {
	auto playLayer = gd::GameManager::sharedState()->getPlayLayer();
	if (playLayer && !playLayer->m_hasCompletedLevel) {
		if (startPosSwitcherLabel) {
			auto fadeout = CCSequence::create(CCDelayTime::create(2.f), CCFadeOut::create(0.5f), nullptr);
			if (!fadeOutFlag)
			{
				startPosSwitcherLabel->runAction(fadeout);
				fadeOutFlag = !fadeOutFlag;
			}
			if (startPosArray->count() > 1) {
				if (currentStartPos == 0) currentStartPos = startPosArray->count() - 1;
				else currentStartPos--;
				if (currentStartPos != 0) {
					playLayer->m_playerStartPosition = reinterpret_cast<gd::StartPosObject*>(startPosArray->objectAtIndex(currentStartPos))->getOrientedBox()->getCenterPoint();
					playLayer->setStartPosObject(reinterpret_cast<gd::StartPosObject*>(startPosArray->objectAtIndex(currentStartPos)));
				}
				else {
					playLayer->m_playerStartPosition = ccp( 0, 105 );
					playLayer->setStartPosObject(nullptr);
				}
				if (currentStartPos == 0) {
					from<bool>(playLayer, 0x2b8) = false;
				}
				else {
					from<bool>(playLayer, 0x2b8) = true;
				}
				playLayer->resetLevel();
				startPosSwitcherLabel->setString(CCString::createWithFormat("%d/%d", currentStartPos, startPosArray->count() - 1)->getCString());
				startPosSwitcherLabel->stopAllActions();
				startPosSwitcherLabel->setOpacity(255);
				startPosSwitcherLabel->runAction(fadeout);
			}
		}
	}
}

void PlayLayer::onPrevStartPos() {
	if (gd::GameManager::sharedState()->getPlayLayer() && !gd::GameManager::sharedState()->getPlayLayer()->m_hasCompletedLevel) {
		if (startPosSwitcherLabel) {
			auto fadeout = CCSequence::create(CCDelayTime::create(2.f), CCFadeOut::create(0.5f), nullptr);
			if (!fadeOutFlag)
			{
				startPosSwitcherLabel->runAction(fadeout);
				fadeOutFlag = !fadeOutFlag;
			}
			if (startPosObjects.size() > 1) {
				if (currentStartPos == 0) currentStartPos = startPosObjects.size() - 1;
				else currentStartPos--;
				if (currentStartPos != 0) {
					//gd::GameManager::sharedState()->getPlayLayer()->m_playerStartPosition = reinterpret_cast<gd::StartPosObject*>(startPosObjects[currentStartPos])->getPosition();
					gd::GameManager::sharedState()->getPlayLayer()->m_playerStartPosition = reinterpret_cast<gd::StartPosObject*>(startPosObjects[currentStartPos])->getOrientedBox()->getCenterPoint();
					gd::GameManager::sharedState()->getPlayLayer()->setStartPosObject(reinterpret_cast<gd::StartPosObject*>(startPosObjects[currentStartPos]));
				}
				else {
					gd::GameManager::sharedState()->getPlayLayer()->m_playerStartPosition = ccp( 0, 105 );
					gd::GameManager::sharedState()->getPlayLayer()->setStartPosObject(nullptr);
				}
				if (currentStartPos == 0) {
					gd::GameManager::sharedState()->getPlayLayer()->m_isTestMode = false;
				}
				else {
					gd::GameManager::sharedState()->getPlayLayer()->m_isTestMode = true;
				}
				gd::GameManager::sharedState()->getPlayLayer()->resetLevel();
				//startPosSwitcherLabel->setString(CCString::createWithFormat("%d/%d", currentStartPos, startPosObjects.size() - 1)->getCString());
				//startPosSwitcherLabel->stopAllActions();
				//startPosSwitcherLabel->setOpacity(255);
				//startPosSwitcherLabel->runAction(fadeout);
			}
		}
	}
}

bool __fastcall PlayLayer::initH(gd::PlayLayer* self, void*, gd::GJGameLevel* level) {
	startPosObjects.clear();
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

	//auto firstStartPos = gd::StartPosObject::create();
	//startPosObjects.push_back(firstStartPos);

	currentStartPos = startPosObjects.size() - 1;
	if (setting().onStartPosSwitcher)
	{
		startPosSwitcherLabel = CCLabelBMFont::create("", "bigFont.fnt");
		startPosSwitcherLabel->setZOrder(5);
		startPosSwitcherLabel->setScale(0.5f);
		startPosSwitcherLabel->setAnchorPoint({ 0.5f, 0.5f });
		startPosSwitcherLabel->setString(CCString::createWithFormat("%d/%d", currentStartPos, startPosObjects.size() - 1)->getCString());
		startPosSwitcherLabel->setPosition({ CCDirector::sharedDirector()->getScreenRight() / 2, 20.f });
		if (startPosObjects.size() == 1) startPosSwitcherLabel->setVisible(0);
		self->addChild(startPosSwitcherLabel);
		if (startPosSwitcherLabel) {
			auto fadeout = CCSequence::create(CCDelayTime::create(2.f), CCFadeOut::create(0.5f), nullptr);
			if (!fadeOutFlag)
			{
				startPosSwitcherLabel->runAction(fadeout);
				fadeOutFlag = !fadeOutFlag;
			}
			startPosSwitcherLabel->stopAllActions();
			startPosSwitcherLabel->setOpacity(255);
			startPosSwitcherLabel->runAction(fadeout);
		}
	}

	if (setting().onHidePlayer) {
		self->m_player->setVisible(0);
		self->m_player2->setVisible(0);
	}
	else {
		self->m_player->setVisible(1);
		self->m_player2->setVisible(1);
	}

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
		if (gd::GameManager::sharedState()->getGameVariable("0040")) {
			percentLabel->setVisible(1);
		}
		else {
			percentLabel->setVisible(0);
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

	if (setting().onHidePlayer) {
		self->m_player->setVisible(0);
		self->m_player2->setVisible(0);
	}
	else {
		self->m_player->setVisible(1);
		self->m_player2->setVisible(1);
	}

	if (setting().onAutoSafeMode && setting().cheatsCount > 0) safeModeON(), setting().isSafeMode = true;
	else if (!setting().onSafeMode) safeModeOFF(), setting().isSafeMode = false;
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

	switch (from<int>(obj, 0x310))
	{
	case 31:
		startPosObjects.push_back(dynamic_cast<gd::StartPosObject*>(obj));
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
}