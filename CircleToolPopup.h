#pragma once
#include "pch.h"
#include "nodes.hpp"

class CircleToolPopup : public gd::FLAlertLayer, gd::FLAlertLayerProtocol, gd::TextInputDelegate {
public:
	CCLabelBMFont* m_label = nullptr;

	static CircleToolPopup* create() {
		CircleToolPopup* node = new (std::nothrow) CircleToolPopup();
		if (node && node->init()) {
			node->autorelease();
		}
		else {
			delete node;
			node = nullptr;
		}
		return node;
	}

	bool init();
	virtual void keyBackClicked() override;
	void onClose(CCObject*);
	void updateLabels();
	void onApply(CCObject*);
	void perform();
	void FLAlert_Clicked(gd::FLAlertLayer*, bool btn2) override {
		if (btn2)
			perform();
	}
	void onOriginalAuthor(CCObject*) {
		CCApplication::sharedApplication()->openURL("https://github.com/matcool/small-gd-mods/blob/main/src/circle-tool.cpp");
	}
};