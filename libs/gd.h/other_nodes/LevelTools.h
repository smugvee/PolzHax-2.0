#ifndef __LEVELTOOLS_H__
#define __LEVELTOOLS_H__

#include <gd.h>

namespace gd {
    class LevelTools {
    public:
        static float xPosForTime(float p0, cocos2d::CCArray* p1, int p2) {
            return reinterpret_cast<float(__fastcall*)(float, cocos2d::CCArray*, int)>(base + 0x1175f0)(p0, p1, p2);
        }

        static float timeForXPos(float p0, cocos2d::CCArray* p1, int p2) {
            return reinterpret_cast<float(__fastcall*)(float, cocos2d::CCArray*, int)>(base + 0x117470)(p0, p1, p2);
        }
    };
}

#endif