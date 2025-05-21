#ifndef __GJBASEGAMELAYER_H__
#define __GJBASEGAMELAYER_H__

#include <gd.h>

namespace gd {
	class PlayerObject;
	class LevelSettingsObject;

	class GJBaseGameLayer : public cocos2d::CCLayer {
	public:
		cocos2d::CCLayer* m_objectLayer; // 0x118
		//cocos2d::CCSpriteBatchNode* m_batchNode; // 0x11c
		//cocos2d::CCSpriteBatchNode* m_batchNodeBottom; // 0x120
		//cocos2d::CCSpriteBatchNode* m_batchNodeBottom2; // 0x124
		//cocos2d::CCSpriteBatchNode* m_batchNodeAdd; // 0x128
		//cocos2d::CCSpriteBatchNode* m_batchNodeAddBottom; // 0x12c
		//cocos2d::CCSpriteBatchNode* m_batchNodeAddBottom2; // 0x130
		//cocos2d::CCSpriteBatchNode* m_batchNodeAddTop; // 0x134
		//cocos2d::CCSpriteBatchNode* m_batchNodePlayer; // 0x138 // also used for old triggers
		//cocos2d::CCSpriteBatchNode* m_batchNodeAddPlayer; // 0x13c
		//cocos2d::CCSpriteBatchNode* m_batchNodeAddMid; // 0x140
		//cocos2d::CCSpriteBatchNode* m_batchNodeText; // 0x144
		//cocos2d::CCSpriteBatchNode* m_batchNodeAddText; // 0x148
		//cocos2d::CCSpriteBatchNode* m_effectBatchNode; // 0x14c
		//cocos2d::CCSpriteBatchNode* m_effectBatchNodeAdd; // 0x150
		//cocos2d::CCSpriteBatchNode* m_batchNodeAddGlow; // 0x154
		//cocos2d::CCSpriteBatchNode* m_batchNodeAddBottomGlow; // 0x158
		//cocos2d::CCSpriteBatchNode* m_batchNodeAddBottom2Glow; // 0x15c
		//cocos2d::CCSpriteBatchNode* m_batchNodeAddMidGlow; // 0x160
		PAD(72)
		PlayerObject* m_player; // 0x164
		PlayerObject* m_player2; // 0x168
		LevelSettingsObject* m_levelSettings; // 0x16c
		cocos2d::CCDictionary* m_disabledGroupsDict; // 0x170

		cocos2d::CCArray* getAllObjects() {
			return from<cocos2d::CCArray*>(this, 0x1f8);
		}
	};
}

#endif