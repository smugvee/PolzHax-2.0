#include "EditorOptionsLayer.h"
#include "state.h"
#include "patching.h"
#include "RotateSaws.h"

void EditorOptionsLayer::Callback::onExtraObjectInfo(CCObject*) {
	setting().onExtraObjectInfo = !setting().onExtraObjectInfo;
}

auto extraObjectInfoToggleSpr(CCSprite* on, CCSprite* off) {
	return (setting().onExtraObjectInfo) ? on : off;
}

void EditorOptionsLayer::Callback::onSelectFilter(CCObject*) {
	setting().onSelectFilter = !setting().onSelectFilter;
}

auto selectFilterToggleSpr(CCSprite* on, CCSprite* off) {
	return (setting().onSelectFilter) ? on : off;
}

void EditorOptionsLayer::Callback::onUnusedObjects(CCObject*) {
	setting().onUnusedObjects = !setting().onUnusedObjects;
}

auto unusedObjectsToggleSpr(CCSprite* on, CCSprite* off) {
	return (setting().onUnusedObjects) ? on : off;
}

void EditorOptionsLayer::Callback::onPreviewAnimations(CCObject*) {
	setting().onRotateSaws = !setting().onRotateSaws;
	if (setting().onRotateSaws)
		RotateSaws::beginRotations(gd::GameManager::sharedState()->getLevelEditorLayer());
	else
		RotateSaws::stopRotations(gd::GameManager::sharedState()->getLevelEditorLayer());
}

auto previewAnimsToggleSpr(CCSprite* on, CCSprite* off) {
	return (setting().onRotateSaws) ? on : off;
}

void EditorOptionsLayer::Callback::onDurationLines(CCObject*) {
	setting().onDurationLines = !setting().onDurationLines;
}

auto durLinesToggleSpr(CCSprite* on, CCSprite* off) {
	return (setting().onDurationLines) ? on : off;
}

void __fastcall EditorOptionsLayer::initH(gd::EditorOptionsLayer* self) {
	EditorOptionsLayer::init(self);

	std::cout << "EditorOptionsLayer::init" << std::endl;

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto toggleOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
	auto toggleOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");

	auto onExtraObjectInfo = gd::CCMenuItemToggler::create(
		extraObjectInfoToggleSpr(toggleOn, toggleOff),
		extraObjectInfoToggleSpr(toggleOff, toggleOn),
		self,
		menu_selector(EditorOptionsLayer::Callback::onExtraObjectInfo));
	
	onExtraObjectInfo->setPosition({ -160.f, 40.f });
	onExtraObjectInfo->setScale(.8f);
	self->m_pButtonMenu->addChild(onExtraObjectInfo);

	auto onExtraObjectInfoLabel = CCLabelBMFont::create("Extra object info", "bigFont.fnt");
	onExtraObjectInfoLabel->setPosition({ winSize.width / 2.f - 139.f, winSize.height / 2.f + 40.f });
	onExtraObjectInfoLabel->setAnchorPoint({ 0.f ,.5f });
	onExtraObjectInfoLabel->setScale(.385f);
	self->m_pLayer->addChild(onExtraObjectInfoLabel);

	auto onSelectFilter = gd::CCMenuItemToggler::create(
		selectFilterToggleSpr(toggleOn, toggleOff),
		selectFilterToggleSpr(toggleOff, toggleOn),
		self,
		menu_selector(EditorOptionsLayer::Callback::onSelectFilter));

	onSelectFilter->setPosition({ 32.f, 40.f });
	onSelectFilter->setScale(.8f);
	self->m_pButtonMenu->addChild(onSelectFilter);

	auto onSelectFilterLabel = CCLabelBMFont::create("Select filter", "bigFont.fnt");
	onSelectFilterLabel->setPosition({ winSize.width / 2.f + 54.f, winSize.height / 2.f + 40.f });
	onSelectFilterLabel->setAnchorPoint({ 0.f ,.5f });
	onSelectFilterLabel->setScale(.485f);
	self->m_pLayer->addChild(onSelectFilterLabel);

	auto onUnusedObjects = gd::CCMenuItemToggler::create(
		unusedObjectsToggleSpr(toggleOn, toggleOff),
		unusedObjectsToggleSpr(toggleOff, toggleOn),
		self,
		menu_selector(EditorOptionsLayer::Callback::onUnusedObjects));

	onUnusedObjects->setPosition({ -160.f, 0.f });
	onUnusedObjects->setScale(.8f);
	self->m_pButtonMenu->addChild(onUnusedObjects);

	auto onUnusedObjectsLabel = CCLabelBMFont::create("Unused objects", "bigFont.fnt");
	onUnusedObjectsLabel->setPosition({ winSize.width / 2.f - 139.f, winSize.height / 2.f + 0.f });
	onUnusedObjectsLabel->setAnchorPoint({ 0.f, .5f });
	onUnusedObjectsLabel->setScale(.485f);
	self->m_pLayer->addChild(onUnusedObjectsLabel);

	auto onPreviewAnimations = gd::CCMenuItemToggler::create(
		previewAnimsToggleSpr(toggleOn, toggleOff),
		previewAnimsToggleSpr(toggleOff, toggleOn),
		self,
		menu_selector(EditorOptionsLayer::Callback::onPreviewAnimations));

	onPreviewAnimations->setPosition({ 32.f, 0.f });
	onPreviewAnimations->setScale(.8f);
	self->m_pButtonMenu->addChild(onPreviewAnimations);

	auto onPreviewAnimationsLabel = CCLabelBMFont::create("Preview rotations", "bigFont.fnt");
	onPreviewAnimationsLabel->setPosition({ winSize.width / 2.f + 54.f, winSize.height / 2.f + 0.f });
	onPreviewAnimationsLabel->setAnchorPoint({ 0.f, .5f });
	onPreviewAnimationsLabel->setScale(.385f);
	self->m_pLayer->addChild(onPreviewAnimationsLabel);

	auto onDurationLines = gd::CCMenuItemToggler::create(
		durLinesToggleSpr(toggleOn, toggleOff),
		durLinesToggleSpr(toggleOff, toggleOn),
		self,
		menu_selector(EditorOptionsLayer::Callback::onDurationLines));

	onDurationLines->setPosition({ -160.f, -40.f });
	onDurationLines->setScale(.8f);
	self->m_pButtonMenu->addChild(onDurationLines);

	auto onDurationLinesLabel = CCLabelBMFont::create("Duration Lines", "bigFont.fnt");
	onDurationLinesLabel->setPosition({ winSize.width / 2.f - 139.f, winSize.height / 2.f - 40.f });
	onDurationLinesLabel->setAnchorPoint({ 0.f, .5f });
	onDurationLinesLabel->setScale(.485f);
	self->m_pLayer->addChild(onDurationLinesLabel);
}