#pragma once
#include "pch.h"

namespace gd {
	class EditorOptionsLayer : public FLAlertLayer {
	public:
	};
}

namespace EditorOptionsLayer {
	inline void(__thiscall* init)(gd::EditorOptionsLayer*);
	void __fastcall initH(gd::EditorOptionsLayer*);

	class Callback : public gd::EditorOptionsLayer {
	public:
		void onSelectFilter(CCObject*);
		void onExtraObjectInfo(CCObject*);
		void onUnusedObjects(CCObject*);
		void onPreviewAnimations(CCObject*);
	};
}