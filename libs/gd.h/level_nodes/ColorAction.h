#ifndef __COLORACTION_H__
#define __COLORACTION_H__

#include <gd.h>

namespace gd {
    class ColorActionSprite;
    
    class ColorAction : public cocos2d::CCNode {
    public:
        cocos2d::ccColor3B m_color; // 0xe8
        float m_currentOpacity; // 0xec
        float m_deltaTime; // 0xf0
        cocos2d::ccColor3B m_fromColor; // 0xf4
        cocos2d::ccColor3B m_toColor; // 0xf8
        float m_duration; // 0xfc
        bool m_blending; // 0x100
        PAD(0x3)
        int m_playerColor; // 0x104
        int m_colorID; // 0x108
        float m_fromOpacity; // 0x10c
        float m_toOpacity; // 0x110
        cocos2d::ccHSVValue m_copyHSV; // 0x114
        int m_copyID; // 0x124
        bool m_copyColorCalculated; // 0x128
        bool m_copyOpacity; // 0x129
        bool m_copyColorLoop; // 0x12a
        bool m_ignoreCopyColor; // 0x12b
    };
}

#endif