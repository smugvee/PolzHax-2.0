#pragma once
#include "pch.h"

namespace PauseLayer {
	inline void(__thiscall* customSetup)(gd::PauseLayer*);
	void __fastcall customSetupH(gd::PauseLayer* self);

	inline void(__thiscall* togglerMenu)();
	void __fastcall togglerMenuH();

	inline void(__thiscall* onRestart)(CCObject*);
	void __fastcall onRestartH(CCObject*);

	class Callback {
	public:
		void onShowPercentage(CCObject*);
	};

	void mem_init();
}