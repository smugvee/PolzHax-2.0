#include "SetGroupIDLayer.h"

bool __fastcall SetGroupIDLayer::initH(gd::SetGroupIDLayer* self, void*, gd::GameObject* obj, CCArray* objs) {
	if (!SetGroupIDLayer::init(self, obj, objs)) return false;



	return true;
}

void SetGroupIDLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x18b1b0), SetGroupIDLayer::initH, reinterpret_cast<void**>(&SetGroupIDLayer::init));
}