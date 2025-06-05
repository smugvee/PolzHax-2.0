#ifndef __LEVELEDITORLAYER_H__
#define __LEVELEDITORLAYER_H__

#include <gd.h>

namespace gd {
	class GJBaseGameLayer;
	class EditorUI;
	class GJGroundLayer;
	class OBB2D;
	class StartPosObject;
	class DrawGridLayer;
	class GameManager;

	class LevelEditorLayer : public GJBaseGameLayer {
	public:
		PAD(0x4)
		PAD(0x8)
		cocos2d::CCArray* m_objects; // 0x180
		cocos2d::CCArray* m_unkArray; // 0x184
		cocos2d::CCArray* m_unkArray2; // 0x188
		cocos2d::CCArray* m_unkArray3; // 0x18c
		cocos2d::CCArray* m_unkArray4; // 0x190
		cocos2d::CCArray* m_unkArray5; // 0x194
		cocos2d::CCArray* m_unkArray6; // 0x198
		cocos2d::CCDictionary* m_unkDict; // 0x19c
		cocos2d::CCArray* m_unkArray7; // 0x1a0
		PAD(0x4)
		cocos2d::CCDictionary* m_unkDict2; // 0x1a8
		cocos2d::CCArray* m_unkArray8; // 0x1ac
		cocos2d::CCNode* m_unkNode; // 0x1b0
		bool m_dontApplyLayerFilter; // 0x1b4 ???
		PAD(0x3)
		int m_coinCount; // 0x1b8
		PAD(0x8)
		int m_firstVisibleSection; // 0x1c4
		int m_lastVisibleSection; // 0x1c8
		PAD(0x4)
		cocos2d::CCArray* m_unkArray9; // 0x1d0
		cocos2d::CCArray* m_unkArray10; // 0x1d4
		int m_groupIDFilter; // 0x1d8
		StartPosObject* m_currentStartPos; // 0x1dc
		PAD(0x4)
		OBB2D* m_obb2d; // 0x1e4
		OBB2D* m_obb2d_2; // 0x1e8
		cocos2d::CCSprite* m_deathSprite; // 0x1ec
		PAD(24)
		gd::EditorUI* m_editorUI; // 0x208
		cocos2d::CCSprite* m_backgroundSprite; // 0x20c
		cocos2d::CCArray* m_levelSections; // 0x210
		cocos2d::CCArray* m_undoObjects; // 0x214
		cocos2d::CCArray* m_redoObjects; // 0x218
		PAD(0x8)
		int m_objectCount; // 0x224
		DrawGridLayer* m_drawGridLayer; // 0x228
		GJGameLevel* m_level; // 0x22c
		int m_levelDistancel; // 0x230
		int m_playerState; // 0x234
		bool m_dualMode; // 0x238
		PAD(0x3)
		float m_minPortalY; // 0x23c
		float m_maxPortalY; // 0x240
		float m_time; // 0x244
		cocos2d::CCDictionary* m_unkDict3; // 0x248
		bool m_3d; // 0x24c ???
		bool m_previewMode; // 0x24d
		PAD(0x2)
		GJGroundLayer* m_groundLayer; // 0x250

		void updateLevelColors() {
			return reinterpret_cast<void(__thiscall*)(LevelEditorLayer*)>(base + 0xf3ce0)(this);
		}

		void updateGroundWidth() {
			from<GJGroundLayer*>(this, 0x250)->updateGroundWidth();
			if (from<bool>(this, 0x24d)) {
				this->updateLevelColors();
			}
		}

		void updateEditorMode() {
			return reinterpret_cast<void(__thiscall*)(LevelEditorLayer*)>(base + 0xf34b0)(this);
		}

		void pushButton(int button, bool idk) {
			reinterpret_cast<void(__thiscall*)(LevelEditorLayer*, int, bool)>(base + 0xfa480)(this, button, idk);
		}
		void releaseButton(int button, bool idk) {
			reinterpret_cast<void(__thiscall*)(LevelEditorLayer*, int, bool)>(base + 0xfa570)(this, button, idk);
		}

		float timeForXPos(float pos) {
			return reinterpret_cast<float(__thiscall*)(LevelEditorLayer*, float)>(base + 0xf7150)(this, pos);
		}
	};
}

#endif