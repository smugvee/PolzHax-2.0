#pragma once
#include "pch.h"

class FindLevelPopup : public gd::FLAlertLayer, gd::TextInputDelegate {
protected:
	std::string m_inputString; 
	bool init();
	virtual void keyBackClicked() override;
	virtual void textChanged(gd::CCTextInputNode*) override;
	void onSearch(CCObject*);
	void onClose(CCObject*);
	void onResetValue(CCObject*);
public:
	static FindLevelPopup* create() {
		FindLevelPopup* ret = new FindLevelPopup();
		if (ret && ret->init()) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};