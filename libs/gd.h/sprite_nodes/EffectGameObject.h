#ifndef __EFFECTGAMEOBJECT_H__
#define __EFFECTGAMEOBJECT_H__

#include <gd.h>

namespace gd {

	enum class EasingType {
		None = 0,
		EaseInOut = 1,
		EaseIn = 2,
		EaseOut = 3,
		ElasticInOut = 4,
		ElasticIn = 5,
		ElasticOut = 6,
		BounceInOut = 7,
		BounceIn = 8,
		BounceOut = 9,
		ExponentialInOut = 10,
		ExponentialIn = 11,
		ExponentialOut = 12,
		SineInOut = 13,
		SineIn = 14,
		SineOut = 15,
		BackInOut = 16,
		BackIn = 17,
		BackOut = 18,
	};

	class GameObject;

	class EffectGameObject : public GameObject {
	public:
		PAD(0x8)
		cocos2d::ccColor3B m_triggerColor; // 0x3e0;
		PAD(0x1)
		float m_triggerDuration; // 0x3e4
		float m_triggerOpacity; // 0x3e8
		int m_targetGroupID; // 0x3ec
		bool m_tintGround; // 0x3f0
		bool m_playerColor1; // 0x3f1
		bool m_playerColor2; // 0x3f2
		bool m_triggerBlending; // 0x3f3
		cocos2d::CCPoint m_move; // 0x3f4
		EasingType m_easingType; // 0x3fc
		bool m_lockToPlayerX; // 0x400
		bool m_lockToPlayerY; // 0x401
		PAD(0x2)
		float m_fadeInTime; // 0x404
		float m_holdTime; // 0x408
		float m_fadeOutTime; // 0x40c
		int m_pulseMode; // 0x410
		int m_targetType; // 0x414
		cocos2d::ccHSVValue m_hsvValue; // 0x418
		int m_copyColorID; // 0x428
		bool m_copyOpacity; // 0x42c
		bool m_pulseMainOnly; // 0x42d
		bool m_pulseDetailOnly; // 0x42e
		bool m_activateGroup; // 0x42f
		float m_spawnDelay; // 0x430
		float m_effectSpawnPos; // 0x434
		int m_effectOrder; // 0x438
		EffectGameObject* m_nextObject; // 0x43c
		cocos2d::ccColor3B m_fromColor; // 0x440
		PAD(1)
		cocos2d::ccColor3B m_toColor; // 0x444
		PAD(1)
		float m_fromOpacity; // 0x448
		float m_toOpacity; // 0x44c
		float m_startTime; // 0x450
		bool m_wasProcessed; // 0x454
		PAD(0x3)
	};
}

#endif