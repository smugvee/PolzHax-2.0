#pragma once
#include "pch.h"

namespace EditorPauseLayer {
	inline void(__thiscall* customSetup)(gd::EditorPauseLayer*);
	void __fastcall customSetupH(gd::EditorPauseLayer*);

	inline void(__thiscall* dtor)(gd::EditorPauseLayer*);
	void __fastcall dtorH(gd::EditorPauseLayer*);

	inline void(__thiscall* keyDown)(gd::EditorPauseLayer*, enumKeyCodes);
	void __fastcall keyDownH(gd::EditorPauseLayer*, void*, enumKeyCodes);

	class Callback {
	public:
		void onSaveLevel(CCObject*);
		void onPasteString(CCObject*);
		void onSelectAll(CCObject*);
	};

	void mem_init();
}