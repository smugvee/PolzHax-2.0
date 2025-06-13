#ifndef __LEVELBROWSERLAYER_H__
#define __LEVELBROWSERLAYER_H__

#include <gd.h>

namespace gd {
	class TextArea;
	class GJListLayer;
	class CCMenuItemSpriteExtra;
	class GJSearchObject;
	class LoadingCircle;

    class LevelBrowserLayer : public cocos2d::CCLayer, LevelManagerDelegate, FLAlertLayerProtocol {
	public:
		PAD(0x4)
		TextArea* m_noInternet; // 0x124
		GJListLayer* m_list; // 0x128
		CCMenuItemSpriteExtra* m_rightArrow; // 0x12c
		CCMenuItemSpriteExtra* m_leftArrow; // 0x130
		PAD(0x4)
		cocos2d::CCArray* m_array; // 0x138
		GJSearchObject* m_searchObject; // 0x13c
		cocos2d::CCLabelBMFont* m_countText; // 0x140
		int m_itemCount; // 0x144
		int m_pageStartIdx; // 0x148
		int m_pageEndIdx; // 0x14c
		LoadingCircle* m_loadingCircle; // 0x150

		void loadPage(GJSearchObject* searchObject) {
			reinterpret_cast<void(__thiscall*)(LevelBrowserLayer*, GJSearchObject*)>(base + 0xeb090)(this, searchObject);
		}
	};
}

#endif