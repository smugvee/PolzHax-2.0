#ifndef __GJEFFECTMANAGER_H__
#define __GJEFFECTMANAGER_H__

#include <gd.h>

namespace gd {
    class ColorAction;
    
    class GJEffectManager : public cocos2d::CCNode {
    public:
        ColorAction* getColorAction(int num) {
            return reinterpret_cast<ColorAction*(__thiscall*)(
                GJEffectManager*, int
            )>(
                base + 0xc8940
            )(
                this, num
            );
        }
    };
}

#endif