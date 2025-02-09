#ifndef __EDITORUI_H__
#define __EDITORUI_H__

#include <gd.h>

namespace gd {
    class CCBlockLayer;

    class EditButtonBar : public cocos2d::CCNode {
    public:

    };

    class EditorPauseLayer : public CCBlockLayer {
    public:
        void saveLevel() {
            reinterpret_cast<void(__fastcall*)(EditorPauseLayer*)>(base + 0x5c290)(this);
        }
    };

    class EditorUI : public cocos2d::CCLayer {
    public:

    };
}

#endif