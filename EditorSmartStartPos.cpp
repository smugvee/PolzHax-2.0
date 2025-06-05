//#include "EditorSmartStartPos.h"
//#include "LevelEditorLayer.h"
//
//std::vector<gd::StartPosObject*> m_startPositions;
//std::vector<gd::GameObject*> m_dualPortals, m_gamemodePortals, m_miniPortals, m_speedChanges, m_mirrorPortals;
//
//static gd::GameObject* getClosestObject(std::vector<gd::GameObject*>& vec, gd::StartPosObject* startPos) {
//	gd::GameObject* closest = nullptr;
//
//	std::ranges::sort(vec, [](gd::GameObject* a, gd::GameObject* b) {
//		return a->getPositionX() < b->getPositionX();
//		});
//
//	for (auto obj : vec) {
//		if (obj->getPositionX() - 10 > startPos->getPositionX())	
//			break;
//		if (obj->getPositionX() - 10 < startPos->getPositionX())
//			closest = obj;
//	}
//
//	return closest;
//}
//
//void EditorSmartStartPos::setupStartPos(gd::StartPosObject* startPos) {
//	gd::LevelSettingsObject* startPosSettings = startPos->m_levelSettings;
//	gd::LevelSettingsObject* levelSettings = gd::GameManager::sharedState()->getPlayLayer()->m_levelSettings;
//
//	startPosSettings->m_startDual = levelSettings->m_startDual;
//	startPosSettings->m_startMode = levelSettings->m_startMode;
//	startPosSettings->m_startMini = levelSettings->m_startMini;
//	startPosSettings->m_startSpeed = levelSettings->m_startSpeed;
//
//	gd::GameObject* obj = getClosestObject(m_dualPortals, startPos);
//	if (obj)
//		startPosSettings->m_startDual = obj->m_objectID == 286;
//
//	obj = getClosestObject(m_gamemodePortals, startPos);
//
//	if (obj) {
//		switch (obj->m_objectID) {
//		case 12: startPosSettings->m_startMode = 0;
//			break;
//		case 13: startPosSettings->m_startMode = 1;
//			break;
//		case 47: startPosSettings->m_startMode = 2;
//			break;
//		case 111: startPosSettings->m_startMode = 3;
//			break;
//		case 660: startPosSettings->m_startMode = 4;
//			break;
//		case 745: startPosSettings->m_startMode = 5;
//			break;
//		default: break;
//		}
//	}
//
//	obj = getClosestObject(m_miniPortals, startPos);
//
//	if (obj)
//		startPosSettings->m_startMini = obj->m_objectID == 101;
//
//	obj = getClosestObject(m_speedChanges, startPos);
//	if (obj) {
//		switch (obj->m_objectID) {
//		case 200: startPosSettings->m_startSpeed = 1;
//			break;
//		case 201: startPosSettings->m_startSpeed = 0;
//			break;
//		case 202: startPosSettings->m_startSpeed = 2;
//			break;
//		case 203: startPosSettings->m_startSpeed = 3;
//			break;
//		default: break;
//		}
//	}
//}
//
//void objectMoved(gd::GameObject* obj) {
//	if (obj->m_objectID != 31) return;
//	EditorSmartStartPos::setupStartPos(static_cast<gd::StartPosObject*>(obj));
//}