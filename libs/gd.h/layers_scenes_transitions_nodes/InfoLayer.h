#ifndef __INFOLAYER_H__
#define __INFOLAYER_H__

#include <gd.h>

namespace gd {
    class GJGameLevel;

    class InfoLayer : public FLAlertLayer, LevelCommentDelegate, CommentUploadDelegate, FLAlertLayerProtocol {
    public:
        GJGameLevel* m_level; // 0x1d4
		gd::string m_commentsKey; // 0x1d8
		LoadingCircle* m_loadingCircle; // 0x1f0
		cocos2d::CCLabelBMFont* m_noComments; // 0x1f4
		cocos2d::CCLayer* m_list; // 0x1f8
		CCMenuItemSpriteExtra* m_rightArrow; // 0x1fc
		CCMenuItemSpriteExtra* m_leftArrow; // 0x200
		ButtonSprite* m_likeBtn; // 0x204
		ButtonSprite* m_timeBtn; // 0x208
		CCMenuItemSpriteExtra* m_reportBtn; // 0x20c
		CCMenuItemSpriteExtra* m_loadCommentsBtn; // 0x210
		int m_itemCount; // 0x214
		int m_pageStartIdx; // 0x218
		int m_pageEndIdx; // 0x21c
		int m_page; // 0x220
		bool m_canUpdateUserScore; // 0x214

        void loadPage(int page) {
			reinterpret_cast<void(__thiscall*)(InfoLayer*, int)>(base + 0xe7360)(this, page);
		}
    };
}

#endif