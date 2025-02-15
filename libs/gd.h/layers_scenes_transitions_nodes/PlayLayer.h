#ifndef __PLAYLAYER_H__
#define __PLAYLAYER_H__

#include <gd.h>

namespace gd {

	class GameObject;
	class GJBaseGameLayer;
	class StartPosObject;
	class GJGameLevel;
	class CCMenuItemSpriteExtra;
	using EndPortalObject = GameObject;
	class AudioEffectsLayer : public cocos2d::CCNode {};
	class GJGroundLayer {
	public:
		void updateGroundWidth() {
			return reinterpret_cast<void(__thiscall*)(GJGroundLayer*)>(base + 0xd2610)(this);
		}
	};

	class UILayer : public cocos2d::CCLayerColor {
	public:
		cocos2d::CCMenu* m_checkpointMenu; // 0x19c
		CCMenuItemSpriteExtra* m_pauseButton; // 0x1a0
	};

	class PlayLayer : public GJBaseGameLayer {
	public:
		PAD(24)
		cocos2d::CCDrawNode* m_unkDrawNode; // 0x18c
		PAD(36)
		StartPosObject* m_startPosObject; // 0x1b4
		EndPortalObject* m_endPortal; // 0x1b8
		cocos2d::CCArray* m_checkpoints; // 0x1bc
		cocos2d::CCArray* m_speedObjects; // 0x1c0
		cocos2d::CCArray* m_allSpeedObjects; // 0x1c4
		PAD(64)
		cocos2d::CCParticleSystemQuad* m_particleSystem; // 0x208
		cocos2d::CCDictionary* m_pickedUpItems; // 0x20c
		PAD(4)
		AudioEffectsLayer* m_audioEffectsLayer; // 0x214
		PAD(8)
		GJGroundLayer* m_bottomGround; // 0x220
		GJGroundLayer* m_topGround; // 0x224
		PAD(8)
		bool m_isDead; // 0x230
		bool m_startCameraAtCorner; // 0x231
		bool m_cameraYLocked; // 0x232
		bool m_cameraXLocked; // 0x233
		PAD(28)
		float m_levelLength; // 0x250
		float m_realLevelLength; // 0x254
		cocos2d::CCLabelBMFont* m_attemptLabel; // 0x258
		cocos2d::CCLabelBMFont* m_percentLabel; // 0x25c
		bool m_isCameraShaking; // 0x260
		float m_currentShakeStrength; // 0x264
		PAD(16)
		cocos2d::CCDictionary* m_particleSystemPlist; // 0x278
		PAD(4)
		cocos2d::CCArray* m_particleSystems; // 0x280
		cocos2d::CCNode* m_backgroundFlashNode; // 0x284
		cocos2d::CCSprite* m_sliderGrooveSprite; // 0x288
		cocos2d::CCSprite* m_sliderBarSprite; // 0x28c
		PAD(24)
		cocos2d::CCArray* m_gravitySprites; // 0x2a8
		PAD(96)
		UILayer* m_uiLayer; // 0x30c
		GJGameLevel* m_level; // 0x310
		cocos2d::CCPoint m_cameraPosition; // 0x314
		bool m_isTestMode; // 0x31c
		bool m_isPracticeMode; // 0x31d
		PAD(2)
		PAD(8)
		cocos2d::CCPoint m_playerStartPosition; // 0x328
		int m_currentAttempt; // 0x330
		int m_jumpsCount; // 0x334
		PAD(4)
		float m_totalTime; // 0x33c
		PAD(1)
		bool m_hasLevelCompletedMenu; // 0x341
		bool m_hasCompletedLevel; // 0x342
		bool m_delayedResetLevel; // 0x343
		int m_lastDeathPercent; // 0x344

		void resetLevel() {
			return reinterpret_cast<void(__thiscall*)(PlayLayer*)>(base + 0x17c930)(this);
		}

		void onQuit() {
			return reinterpret_cast<void(__thiscall*)(PlayLayer*)>(base + 0x17de80)(this);
		}

		void resume() {
			return reinterpret_cast<void(__thiscall*)(PlayLayer*)>(base + 0x17dcb0)(this);
		}

		void setStartPosObject(StartPosObject* startPos) {
			if (startPos != m_startPosObject) {
				if (startPos) {
					reinterpret_cast<CCObject*>(startPos)->retain();
				}
				if (m_startPosObject) {
					reinterpret_cast<CCObject*>(m_startPosObject)->release();
				}
				m_startPosObject = startPos;
			}
		}
	};
}

#endif