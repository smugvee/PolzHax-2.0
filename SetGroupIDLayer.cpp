#include "SetGroupIDLayer.h"
#include "utils.hpp"
#include "state.h"

class OffsetGroupInput : public cocos2d::CCLayer, gd::TextInputDelegate {
protected:
	bool init() {
		if (!CCLayer::init()) return false;

		auto textInput = gd::CCTextInputNode::create("ID", this, "bigFont.fnt", 28.f, 18.f);
		textInput->setMaxLabelLength(3);
		textInput->setMaxLabelWidth(28.f);
		textInput->setAllowedChars("0123456789");
		textInput->setLabelPlaceholderColor({ 120, 170, 240 });
		textInput->setLabelPlaceholerScale(.5f);
		textInput->setMaxLabelScale(.5f);
		textInput->setDelegate(this);
		textInput->setString(std::to_string(setting().groupIDOffset).c_str());
		this->addChild(textInput, 20);

		return true;
	}

	virtual void textChanged(gd::CCTextInputNode* input) override {
		setting().groupIDOffset = std::atoi(input->getString());
		
		auto bytes = intToBytes(setting().groupIDOffset);
		bytes.insert(bytes.begin(), 0xbe);

		sequence_patch(uint32_t(gd::base + 0xf2f7f), bytes);

		std::cout << "Offset: " << setting().groupIDOffset << std::endl;
	}

public:
	static OffsetGroupInput* create() {
		OffsetGroupInput* ret = new OffsetGroupInput();
		if (ret && ret->init()) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};

bool __fastcall SetGroupIDLayer::initH(gd::SetGroupIDLayer* self, void*, gd::GameObject* obj, CCArray* objs) {
	if (!SetGroupIDLayer::init(self, obj, objs)) return false;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto offsetInputBG = extension::CCScale9Sprite::create("square02_small.png");
	offsetInputBG->setOpacity(100);
	offsetInputBG->setScale(.7f);
	offsetInputBG->setContentSize({ 45.f, 28.f });
	offsetInputBG->setPosition(winSize.width / 2.f - 83.f, winSize.height / 2.f + 43.f);
	self->m_pLayer->addChild(offsetInputBG, -1);

	auto offsetLabel = CCLabelBMFont::create("Offset", "goldFont.fnt");
	offsetLabel->setScale(.4f);
	offsetLabel->setPosition(offsetInputBG->getPositionX(), offsetInputBG->getPositionY() + 17.f);
	self->m_pLayer->addChild(offsetLabel);

	auto offsetInput = OffsetGroupInput::create();
	offsetInput->setPosition(offsetInputBG->getPosition());
	self->m_pLayer->addChild(offsetInput);

	return true;
}

void SetGroupIDLayer::mem_init() {
	MH_CreateHook(reinterpret_cast<void*>(gd::base + 0x18b1b0), SetGroupIDLayer::initH, reinterpret_cast<void**>(&SetGroupIDLayer::init));
}