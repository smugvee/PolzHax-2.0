#include "CircleToolPopup.h"

static float m_angle;
static float m_step;

bool CircleToolPopup::init() {
	if (!this->initWithColor({ 0, 0, 0, 75 })) return false;

	this->m_bNoElasticity = true;

	auto* director = CCDirector::sharedDirector();
	director->getTouchDispatcher()->incrementForcePrio(69);
	this->registerWithTouchDispatcher();

	auto layer = CCLayer::create();
	auto menu = CCMenu::create();
	this->m_pLayer = layer;
	this->m_pButtonMenu = menu;

	layer->addChild(menu);
	this->addChild(layer);

	const float width = 300, height = 220;

	const CCPoint offset = director->getWinSize() / 2.f;
	auto bg = extension::CCScale9Sprite::create("GJ_square01.png");
	bg->setContentSize({ width, height });
	bg->setPosition(offset);
	menu->setPosition(offset);
	bg->setZOrder(-2);
	layer->addChild(bg);

	auto close_btn = gd::CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png"), nullptr,
		this, menu_selector(CircleToolPopup::onClose)
	);

	close_btn->setPosition({-145, 105});
	menu->addChild(close_btn);

	this->setKeypadEnabled(true);


	layer->addChild(
		NodeFactory<CCLabelBMFont>::start("Circle Tool", "goldFont.fnt")
		.setPosition(ccp(0, 95) + offset)
		.setScale(0.75f)
	);

	layer->addChild(
		NodeFactory<CCLabelBMFont>::start("Arc", "goldFont.fnt")
		.setPosition(ccp(-60, 64) + offset)
		.setScale(0.75f)
	);
	auto angle_input = FloatInputNode::create(CCSize(60, 30), 2.f, "bigFont.fnt");
	angle_input->set_value(m_angle);
	angle_input->setPosition(ccp(-60, 38) + offset);
	angle_input->callback = [&](FloatInputNode& input) {
		m_angle = input.get_value().value_or(m_angle);
		this->updateLabels();
		};
	layer->addChild(angle_input);

	layer->addChild(
		NodeFactory<CCLabelBMFont>::start("Step", "goldFont.fnt")
		.setPosition(ccp(60, 64) + offset)
		.setScale(0.75f)
	);
	auto step_input = FloatInputNode::create(CCSize(60, 30), 2.f, "bigFont.fnt");
	step_input->set_value(m_step);
	step_input->setPosition(ccp(60, 38) + offset);
	step_input->callback = [&](FloatInputNode& input) {
		m_step = input.get_value().value_or(m_step);
		if (m_step == 0.f) m_step = 1.f;
		this->updateLabels();
		};
	layer->addChild(step_input);

	auto apply_btn = gd::CCMenuItemSpriteExtra::create(
		gd::ButtonSprite::create("Apply", 0, false, "goldFont.fnt", "GJ_button_01.png", 0, 0.75f), nullptr,
		this, menu_selector(CircleToolPopup::onApply)
	);
	apply_btn->setPosition({ 0, -85 });
	menu->addChild(apply_btn);

	m_label = CCLabelBMFont::create("copies: 69\nobjects: 69420", "chatFont.fnt");
	m_label->setAlignment(kCCTextAlignmentLeft);
	m_label->setPosition(ccp(-83, -41) + offset);
	layer->addChild(m_label);
	this->updateLabels();

	auto originalAuthorLbl = CCLabelBMFont::create("Original by Mat", "goldFont.fnt");
	originalAuthorLbl->setScale(0.5f);
	auto originalAuthor = gd::CCMenuItemSpriteExtra::create(originalAuthorLbl, nullptr, this, menu_selector(CircleToolPopup::onOriginalAuthor));
	originalAuthor->setPosition(ccp(-92, -96));
	menu->addChild(originalAuthor);

	this->setTouchEnabled(true);

	return true;
}

void CircleToolPopup::keyBackClicked() {
	this->setTouchEnabled(false);
	this->removeFromParentAndCleanup(true);
}

void CircleToolPopup::onClose(CCObject*) {
	this->keyBackClicked();
}

void CircleToolPopup::updateLabels() {
	auto objs = gd::GameManager::sharedState()->getLevelEditorLayer()->m_editorUI->m_selectedObjects;
	const auto amt = static_cast<size_t>(std::ceilf(m_angle / m_step) - 1.f);
	const auto obj_count = amt * objs->count();
	m_label->setString(("Copies: " + std::to_string(amt) + "\nObjects: " + std::to_string(obj_count)).c_str());
}

void CircleToolPopup::onApply(CCObject*) {
	auto* editor = gd::GameManager::sharedState()->getLevelEditorLayer()->m_editorUI;
	auto objs = editor->m_selectedObjects;
	const auto amt = static_cast<size_t>(std::ceilf(m_angle / m_step) - 1.f);
	if (objs && objs->count()) {
		const auto obj_count = objs->count() * amt;
		if (obj_count > 5000) {
			gd::FLAlertLayer::create(this, "Warning", "Cancel", "Ok", "This will create <cy>" + std::to_string(obj_count) + "</c> objects, are you sure?")->show();
		}
		else {
			perform();
		}
	}
}

void CircleToolPopup::perform() {
	auto* editor = gd::GameManager::sharedState()->getLevelEditorLayer();
	auto* editor_ui = editor->m_editorUI;
	auto* objs = CCArray::create();
	for (float i = 1; i * m_step < m_angle; ++i) {
		editor_ui->onDuplicate(nullptr);
		auto selected = editor_ui->m_selectedObjects;
		editor_ui->rotateObjects(selected, m_step, { 0.f, 0.f });

		const float angle = i * m_step;

		editor->m_undoObjects->removeLastObject();
		objs->addObjectsFromArray(selected);
	}
	editor->m_undoObjects->addObject(gd::UndoObject::createWithArray(objs, gd::UndoCommand::Paste));
	editor_ui->selectObjects(objs);
	this->keyBackClicked();
}