#pragma once
#include "pch.h"

namespace LevelEditorLayer {
	
	void mem_init();
}

namespace DrawGridLayer {
	inline void(__thiscall* draw)(gd::DrawGridLayer*);
	void __fastcall drawH(gd::DrawGridLayer*);
}