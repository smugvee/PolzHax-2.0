#ifndef __LEVELEDITORLAYER_H__
#define __LEVELEDITORLAYER_H__

#include <gd.h>

namespace gd {
    class GJBaseGameLayer;
    class EditorUI;
    class GJGroundLayer;

    class LevelEditorLayer : public GJBaseGameLayer {
    public:
        PAD(0x4)
        cocos2d::CCArray* m_objects; // 0x180
        cocos2d::CCArray* m_unkArray; // 0x184
        cocos2d::CCArray* m_unkArray2; // 0x188
        cocos2d::CCArray* m_unkArray3; // 0x18c
        cocos2d::CCArray* m_unkArray4; // 0x190
        cocos2d::CCArray* m_unkArray5; // 0x194
        cocos2d::CCArray* m_unkArray8; // 0x198
        cocos2d::CCDictionary* m_unkDict; // 0x19c
        cocos2d::CCArray* m_unkArray6; // 0x1a0
        PAD(0x4)
        cocos2d::CCDictionary* m_unkDict2; // 0x1a8
        cocos2d::CCArray* m_unkArray7; // 0x1ac
        cocos2d::CCNode* m_unkNode; // 0x1b0


        gd::EditorUI* m_editorUI; // 0x208

        void updateLevelColors() {
            return reinterpret_cast<void(__thiscall*)(LevelEditorLayer*)>(base + 0xf3ce0)(this);
        }

        void updateGroundWidth() {
            from<GJGroundLayer*>(this, 0x250)->updateGroundWidth();
            if (from<bool>(this, 0x24d)) {
                this->updateLevelColors();
            }
        }


    };
}

#endif