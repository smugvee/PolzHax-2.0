#ifndef __LEVELSETTINGSOBJECT_H__
#define __LEVELSETTINGSOBJECT_H__

#include <gd.h>

namespace gd {
	class GameObject;
	class GJEffectManager;

	class LevelSettingsObject : public cocos2d::CCNode {
	public:
		GJEffectManager* m_effectManager; // 0xe8
		int m_startMode; // 0xec
		int m_startSpeed; // 0xf0
		bool m_startMini; // 0xf4
		bool m_startDual; // 0xf5
		bool m_twoPlayerMode; // 0xf6
		float m_songOffset; // 0xf8
		bool m_fadeIn; // 0xfc
		bool m_fadeOut; // 0xfd
		int m_backgroundIndex; // 0x100
		int m_groundIndex; // 0x104
		int m_fontIndex; // 0x108
		bool m_startWithStartPos; // 0x10c
		bool m_isFlipped; // 0x10d
		GJGameLevel* m_level; // 0x110
		std::string m_guidelineString; // 0x114
		int m_defaultSongID; // 0x12c
		int m_colorPage; // 0x130
		int m_groundLineIndex; // 0x134
	};
}

#endif