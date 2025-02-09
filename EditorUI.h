#pragma once
#include "pch.h"

namespace EditorPauseLayer {
	inline void(__thiscall* customSetup)(gd::EditorPauseLayer*);
	void __fastcall customSetupH(gd::EditorPauseLayer*);

	class Callback {
	public:
		void onSaveLevel(CCObject*);
	};

	void mem_init();
}