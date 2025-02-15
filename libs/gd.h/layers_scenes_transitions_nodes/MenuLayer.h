#ifndef __MENULAYER_H__
#define __MENULAYER_H__

#include <gd.h>

namespace gd {

	class FLAlertLayerProtocol;

	class MenuLayer : public cocos2d::CCLayer, public gd::FLAlertLayerProtocol {
	public:
		void onOptions(CCObject* sender) {
			return reinterpret_cast<void(__thiscall*)(MenuLayer*, CCObject*)>(base + 0x11e4a0)(this, sender);
		}
	};
}

#endif