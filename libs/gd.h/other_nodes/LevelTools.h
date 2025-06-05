#ifndef __LEVELTOOLS_H__
#define __LEVELTOOLS_H__

#include <gd.h>

namespace gd {
    class LevelTools {
    public:
        static float xPosForTime(float p0, cocos2d::CCArray* p1, int p2) {
            float time;
            __asm movss xmm0, time;
            reinterpret_cast<void(__fastcall*)(cocos2d::CCArray*, int)>(base + 0x117470)(p1, p2);
            __asm movss p0, xmm0;
            return time;
        }

        static float timeForXPos(float pos, cocos2d::CCArray* p1, int speed) {
            float time;
            __asm movss xmm0, pos;
            __asm movss xmm1, speed;
            reinterpret_cast<void(__fastcall*)(cocos2d::CCArray*)>(base + 0x1175f0)(p1);
            __asm movss time, xmm0;
            return time;
        }
    };
}

#endif