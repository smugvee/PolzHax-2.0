#ifndef __UNDOOBJECT_H__
#define __UNDOOBJECT_H__

#include <gd.h>

namespace gd {
    enum UndoCommand {
        Delete = 1,
        New = 2,
        Paste = 3,
        DeleteMulti = 4,
        Transform = 5,
        Select = 6,
    };

    class UndoObject : public cocos2d::CCObject {
        public:
            GameObject* m_pGameObject;      // 0x20
            UndoCommand m_eCommand;         // 0x24
            cocos2d::CCArray* m_pObjects;   // 0x28
            bool m_bRedo;                   // 0x2c

            static UndoObject* create(GameObject* pObject, UndoCommand nCommand) {
                return reinterpret_cast<UndoObject*(__fastcall*)(GameObject*, UndoCommand)>(
                    base + 0x16bc20
                )(pObject, nCommand);
            }
            static UndoObject* createWithArray(cocos2d::CCArray* pObjects, UndoCommand nCommand) {
                return reinterpret_cast<UndoObject*(__fastcall*)(cocos2d::CCArray*, UndoCommand)>(
                    base + 0xfd2b0
                )(pObjects, nCommand);
            }
            static UndoObject* createWithTransformObjects(cocos2d::CCArray* pObjects, UndoCommand nCommand) {
                return reinterpret_cast<UndoObject*(__fastcall*)(cocos2d::CCArray*, UndoCommand)>(
                    base + 0x16bcd0
                )(pObjects, nCommand);
            }
    };
}

#endif