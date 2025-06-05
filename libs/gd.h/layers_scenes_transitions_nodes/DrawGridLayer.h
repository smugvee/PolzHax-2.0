#ifndef __DRAWGRIDLAYER_H__
#define __DRAWGRIDLAYER_H__

#include <gd.h>

namespace gd {
    class DrawGridLayer : public cocos2d::CCLayer {
	public:
		PAD(12)
		float m_songOffset1; // 0x124
		float m_songOffset2; // 0x128
		float m_lastMusicXPos; // 0x12c
		bool m_effectSortDirty; // 0x130
		PAD(0x3)
		LevelEditorLayer* m_editorLayer; // 0x134
		std::string m_guidelineString; // 0x138
		cocos2d::CCLayer* m_gameLayer; // 0x150
		cocos2d::CCArray* m_guidelines; // 0x154
		cocos2d::CCArray* m_effects; // 0x158
		cocos2d::CCArray* m_guides; // 0x15c
		cocos2d::CCArray* m_speedObjects1; // 0x160
		cocos2d::CCArray* m_speedObjects2; // 0x164
		cocos2d::CCArray* m_playerNodePoints; // 0x168
		cocos2d::CCArray* m_player2NodePoints; // 0x16c
		PAD(0x8)
		float m_guidelineSpacing; // 0x178
		float m_slowGuidelineSpacing; // 0x17c
		float m_normalGuidelineSpacing; // 0x180
		float m_fastGuidelineSpacing; // 0x184
		float m_fasterGuidelineSpacing; // 0x188
		bool m_updatingTimeMarkers; // 0x18c
		bool m_timeNeedsUpdate; // 0x18d
		PAD(0x2)
		float m_activeGridNodeSize; // 0x190

		float xPosForTime(float time) {
			__asm movss xmm1, time
			return reinterpret_cast<float(__vectorcall*)(DrawGridLayer*)>(base + 0xfdff0)(this);
		}
	};
}

#endif