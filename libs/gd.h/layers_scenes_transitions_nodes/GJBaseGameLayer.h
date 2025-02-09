#ifndef __GJBASEGAMELAYER_H__
#define __GJBASEGAMELAYER_H__

#include <gd.h>

namespace gd {
	class PlayerObject;
	class LevelSettingsObject;

	class GJBaseGameLayer : public cocos2d::CCLayer {
	public:
		cocos2d::CCLayer* m_objectLayer; // 0x118
		PAD(72)
		PlayerObject* m_player; // 0x164
		PlayerObject* m_player2; // 0x168
		LevelSettingsObject* m_levelSettings; // 0x16c
		cocos2d::CCDictionary* m_disabledGroupsDict; // 0x170
	};
}

#endif