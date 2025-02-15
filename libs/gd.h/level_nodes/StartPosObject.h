#ifndef __STARTPOSOBJECT_H__
#define __STARTPOSOBJECT_H__

#include <gd.h>

namespace gd {

	class LevelSettingsObject;

	class StartPosObject : public GameObject {
	public:
		PAD(28)
		LevelSettingsObject* m_levelSettings; // 0x458
	};
}
#endif
