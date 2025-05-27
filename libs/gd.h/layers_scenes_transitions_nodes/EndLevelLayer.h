#ifndef __ENDLEVELLAYER_H__
#define __ENDLEVELLAYER_H__

#include <gd.h>

namespace gd {
    class EndLevelLayer : public GJDropDownLayer {
    public:
        cocos2d::CCMenu* m_bottomMenu; // 0x1c0
    };
}

#endif