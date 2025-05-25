#ifndef __SETGROUPIDLAYER_H__
#define __SETGROUPIDLAYER_H__

#include <gd.h>

namespace gd {
    class SetGroupIDLayer : public FLAlertLayer, TextInputDelegate {
    public:
        void onZLayer(cocos2d::CCObject* obj) {
            reinterpret_cast<void(__thiscall*)(SetGroupIDLayer*, cocos2d::CCObject*)>(base + 0x18da20)(this, obj);
        }
    };
}

#endif