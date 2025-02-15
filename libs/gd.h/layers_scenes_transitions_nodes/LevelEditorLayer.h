#ifndef __LEVELEDITORLAYER_H__
#define __LEVELEDITORLAYER_H__

#include <gd.h>

namespace gd {
    class GJBaseGameLayer;
    class EditorUI;
    class GJGroundLayer;

    class LevelEditorLayer : public GJBaseGameLayer {
    public:
        PAD(0x94)
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