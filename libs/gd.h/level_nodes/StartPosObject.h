#ifndef __STARTPOSOBJECT_H__
#define __STARTPOSOBJECT_H__

#include <gd.h>

namespace gd {

	class LevelSettingsObject;

	class StartPosObject : public GameObject {
	public:
		LevelSettingsObject* getLevelSettings() {
			return from<LevelSettingsObject*>(this, 0x458);
		}
	};
}
#endif
