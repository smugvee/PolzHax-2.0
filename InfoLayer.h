#pragma once
#include "pch.h"

namespace InfoLayer {
	inline bool(__thiscall* init)(gd::InfoLayer*, gd::GJGameLevel*);
	bool __fastcall initH(gd::InfoLayer* self, void*, gd::GJGameLevel* gameLevel);

	class Callback : public gd::InfoLayer {
	public:
		void onRefreshComments(CCObject*);
	};
}