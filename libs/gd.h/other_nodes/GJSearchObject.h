#ifndef __GJSEARCHOBJECT_H__
#define __GJSEARCHOBJECT_H__

#include <gd.h>

namespace gd {
	enum class SearchType {
		Search = 0,
		Downloaded = 1,
		MostLiked = 2,
		Trending = 3,
		Recent = 4,
		UsersLevels = 5,
		Featured = 6,
		Magic = 7,
		Sends = 8,
		MapPack = 9,
		MapPackOnClick = 10,
		Awarded = 11,
		Followed = 12,
		Friends = 13,
		Users = 14,
		LikedGDW = 15,
		HallOfFame = 16,
		FeaturedGDW = 17,
		Similar = 18,
		MyLevels = 98,
		SavedLevels = 99,
		FavouriteLevels = 100
	};

	class GJSearchObject : public cocos2d::CCNode {
	public:
		SearchType m_type; // 0xe8
		std::string m_searchQuery; // 0xec
		std::string m_difficulty; // 0x104
		std::string m_length; // 0x11c
		int m_page; // 0x134
		bool m_star; // 0x138
		bool m_noStar; // 0x139
		PAD(0x2)
		int m_total; // 0x13c
		bool m_uncompleted; // 0x140
		bool m_featured; // 0x141
		bool m_original; // 0x142
		bool m_twoP; // 0x143
		bool m_coins; // 0x144
		PAD(0x3)
		int m_song; // 0x148
		bool m_customSong; // 0x14c
		bool m_songFilter; // 0x14d

		static GJSearchObject* create(SearchType type, std::string searchQuery, std::string difficulty, std::string length, int page, bool star, bool uncompleted, bool featured, int song, bool original, bool twoP, bool customSong, bool songFilter, bool noStar, bool coins) {
			return reinterpret_cast<GJSearchObject * (__fastcall*)(SearchType, std::string, std::string, std::string, int, bool, bool, bool, int, bool, bool, bool, bool, bool, bool)>(base + 0x990d0)(type, searchQuery, difficulty, length, page, star, uncompleted, featured, song, original, twoP, customSong, songFilter, noStar, coins);
		}

		static GJSearchObject* create(SearchType type, std::string str) {
			auto ret = reinterpret_cast<GJSearchObject * (__fastcall*)(SearchType, std::string)>(base + 0x98fe0)(type, str);
			__asm add esp, 0x18
			return ret;
		}

		static GJSearchObject* create(SearchType type) {
			return reinterpret_cast<GJSearchObject * (__fastcall*)(SearchType)>(base + 0x98f00)(type);
		}

		const char* getKey() {
			return reinterpret_cast<const char* (__fastcall*)(GJSearchObject*)>(base + 0x993b0)(this);
		}
	};
}

#endif