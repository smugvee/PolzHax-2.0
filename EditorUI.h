#pragma once
#include "pch.h"

namespace EditorUI {
	inline bool(__thiscall* init)(gd::EditorUI*, gd::LevelEditorLayer*);
	bool __fastcall initH(gd::EditorUI*, void*, gd::LevelEditorLayer*);

	inline void(__thiscall* createMoveMenu)(gd::EditorUI*);
	void __fastcall createMoveMenuH(gd::EditorUI*);

	inline CCPoint*(__thiscall* moveForCommand)(gd::EditorUI*, CCPoint*, gd::EditCommand);
	CCPoint* __fastcall moveForCommandH(gd::EditorUI* self, void* edx, CCPoint* pos, gd::EditCommand com);

	inline void(__thiscall* transformObject)(gd::EditorUI*, gd::GameObject*, gd::EditCommand, bool);
	void __fastcall transformObjectH(gd::EditorUI* self, void* edx, gd::GameObject* obj, gd::EditCommand com, bool idk);

	inline void(__thiscall* onCopy)(gd::EditorUI*, CCObject*);
	void __fastcall onCopyH(gd::EditorUI*, void*, CCObject*);

	inline void(__thiscall* dtor)(gd::EditorUI*);
	void __fastcall dtorH(gd::EditorUI*);

	inline void(__thiscall* keyDown)(gd::EditorUI*, enumKeyCodes);
	void __fastcall keyDownH(gd::EditorUI*, void*, enumKeyCodes);

	inline void(__thiscall* keyUp)(gd::EditorUI*, enumKeyCodes);
	void __fastcall keyUpH(gd::EditorUI*, void*, enumKeyCodes);

	inline void(__thiscall* scrollWheel)(gd::EditorUI*, float, float);
	void __fastcall scrollWheelH(gd::EditorUI*, void*, float, float);

	class Callback {
	public:
		void onCustomMoveObject(CCObject*);
		void onCustomTransformObject(CCObject*);
	};

	void mem_init();
}

namespace EditorPauseLayer {
	inline void(__thiscall* customSetup)(gd::EditorPauseLayer*);
	void __fastcall customSetupH(gd::EditorPauseLayer*);

	inline void(__thiscall* dtor)(gd::EditorPauseLayer*);
	void __fastcall dtorH(gd::EditorPauseLayer*);

	class Callback {
	public:
		void onSaveLevel(CCObject*);
		void onPasteString(CCObject*);
	};

	void mem_init();
}