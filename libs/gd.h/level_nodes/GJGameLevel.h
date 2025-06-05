#pragma once
#ifndef GJGAMELEVEL_H
#define GJGAMELEVEL_H

#include <gd.h>

namespace gd {
    enum class GJLevelType {
        Default = 0,
        Local = 1,
        Editor = 2,
        Saved = 3
    };

    class GJGameLevel : public cocos2d::CCNode {
    public:
        cocos2d::CCDictionary* m_lastBuildSave; // 0xe8
        int m_levelID; // 0xec
        std::string m_levelName; // 0xf0
        std::string m_levelDesc; // 0x108
        std::string m_levelString; // 0x120
        std::string m_creatorName; // 0x138
        std::string m_recordString; // 0x150
        std::string m_uploadDate; // 0x168
        std::string m_updateDate; // 0x180
        int m_userID; // 0x198
        int m_accountID; // 0x19c
        int m_difficulty; // 0x1a0
        int m_audioTrack; // 0x1a4
        int m_songID; // 0x1a8
        int m_levelRev; // 0x1ac
        int m_objectCount; // 0x1b0
        int m_order; // 0x1b4
        int m_ratings; // 0x1b8
        int m_ratingsSum; // 0x1bc
        int m_downloads; // 0x1c0
        int m_completes; // 0x1c4
        bool m_isEditable; // 0x1c8
        bool m_isVerified; // 0x1c9
        bool m_isUploaded; // 0x1ca
        bool m_hasBeenModified; // 0x1cb
        int m_levelVersion; // 0x1cc
        int m_gameVersion; // 0x1d0
        int m_attempts; // 0x1d4
        int m_jumps; // 0x1d8
        bool m_wasCheatReset; // 0x1dc
        PAD(0x3)
        int m_normalPercent; // 0x1e0
        int m_normalPercentRand1; // 0x1e4
        int m_normalPercentRand2; // 0x1e8
        int m_practicePercent; // 0x1ec
        int m_likes; // 0x1f0
        int m_dislikes; // 0x1f4
        int m_levelLength; // 0x1f8
        int m_featured; // 0x1fc
        bool m_demon; // 0x200
        PAD(0x3)
        int m_stars; // 0x204
        bool m_autoLevel; // 0x208
        PAD(0x3)
        int m_coins; // 0x20c
        int m_coinsVerified; // 0x210
        int m_password; // 0x214
        int m_originalLevel; // 0x218
        bool m_twoPlayerMode; // 0x21c
        PAD(0x3)
        int m_failedPasswordAttempts; // 0x220
        int m_coin1Verified; // 0x224
        int m_coin2Verified; // 0x228
        int m_coint3Verified; // 0x22c
        int m_requestedStars; // 0x230
        bool m_showedSongWarning; // 0x234
        PAD(0x3)
        int m_starRatings; // 0x238
        int m_starRatingsSum; // 0x23c
        int m_maxStarRatings; // 0x240
        int m_minStarRatings; // 0x244
        int m_demonVotes; // 0x248
        int m_rateStars; // 0x24c
        bool m_rateFeature; // 0x250
        PAD(0x3)
        std::string m_rateUser; // 0x254
        bool m_dontSave; // 0x26c
        bool m_isHidden; // 0x26d
        PAD(0x2)
        int m_requiredCoins; // 0x270
        bool m_isUnlocked; // 0x274
        cocos2d::CCPoint m_lastCameraPos; // 0x278
        float m_lastEditorZoom; // 0x280
        int m_lastBuildTab; // 0x284
        int m_lastBuildPage; // 0x288
        int m_lastBuildGroupID; // 0x28c
        GJLevelType m_levelType; // 0x290
        int m_ID; // 0x294
        std::string m_tempName; // 0x298
        std::string m_capacityString; // 0x2b0

    public:
        virtual void encodeWithCoder(DS_Dictionary* p0) {
            return reinterpret_cast<void(__thiscall*)(GJGameLevel*, DS_Dictionary*)>(base + 0x95ab0)(this, p0);
        }
    };
}

#endif