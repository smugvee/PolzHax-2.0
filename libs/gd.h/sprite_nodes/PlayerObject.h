#ifndef __PLAYEROBJECT_H__
#define __PLAYEROBJECT_H__

#include <gd.h>

namespace gd {

	class GameObject;

	class PlayerObject : public GameObject {
	public:
		virtual void setVisible(bool visible) {
			return reinterpret_cast<void(__thiscall*)(PlayerObject*, bool)>(
				base + 0x1FA860
				)(this, visible);
		}
		void pushButton(int button) {
			reinterpret_cast<void(__thiscall*)(PlayerObject*, int)>(base + 0x164e40)(this, button);
		}
		void releaseButton(int button) {
			reinterpret_cast<void(__thiscall*)(PlayerObject*, int)>(base + 0x164f10)(this, button);
		}
	};
}

#endif