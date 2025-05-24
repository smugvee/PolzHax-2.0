#ifndef __HARDSTREAK_H__
#define __HARDSTREAK_H__

namespace gd {
    class HardStreak : public cocos2d::CCDrawNode {
	public:
        PAD(16)
		cocos2d::CCArray* m_pointArray; // 0x118
		cocos2d::CCPoint m_currentPoint; // 0x11c
		float m_waveSize; // 0x124
		float m_pulseSize; // 0x128
		bool m_disableDual; // 0x12c
	};
}


#endif