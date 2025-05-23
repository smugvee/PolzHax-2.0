#pragma once
#include "pch.h"

namespace LevelEditorLayer {
	inline bool(__thiscall* init)(gd::LevelEditorLayer*, gd::GJGameLevel*);
	bool __fastcall initH(gd::LevelEditorLayer*, void*, gd::GJGameLevel*);

	inline void(__thiscall* updateVisibility)(gd::LevelEditorLayer*, float);
	void __fastcall updateVisibilityH(gd::LevelEditorLayer*, void*, float);

	inline void(__thiscall* update)(gd::LevelEditorLayer*, float);
	void __fastcall updateH(gd::LevelEditorLayer*, void*, float);
	
	void mem_init();
}

namespace DrawGridLayer {
	inline void(__thiscall* draw)(gd::DrawGridLayer*);
	void __fastcall drawH(gd::DrawGridLayer*);
}