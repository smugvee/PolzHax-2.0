#ifndef __STARTPOSOBJECT_H__
#define __STARTPOSOBJECT_H__

#include <gd.h>

namespace gd {

	class EffectGameObject;
	class LevelSettingsObject;

	class StartPosObject : public EffectGameObject {
	public:
		LevelSettingsObject* m_levelSettings; // 0x458

		static StartPosObject* create() {
			return reinterpret_cast<StartPosObject * (__fastcall*)()>(base + 0x1a1770)();
		}

		void setSettings(LevelSettingsObject* p0) {
			if (m_levelSettings != p0) {
				if (p0) p0->retain();
				if (m_levelSettings) p0->release();
				m_levelSettings = p0;
			}
			return;
		}
	};
}
#endif
