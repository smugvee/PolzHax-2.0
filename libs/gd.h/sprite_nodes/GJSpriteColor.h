#ifndef __GJSPRITECOLOR_H__
#define __GJSPRITECOLOR_H__

#include <gd.h>

namespace gd {
    class GJSpriteColor : public cocos2d::CCNode {
    public:
        int colorID;
        int defaultColorID;
        PAD(4)
        float hue;
        float saturation;
        float brightness;
        bool saturationChecked;
        bool brightnessChecked;
    };
}

#endif