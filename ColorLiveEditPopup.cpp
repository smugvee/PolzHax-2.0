//#include "ColorLiveEditPopup.h"
//
//ColorLiveEditPopup* ColorLiveEditPopup::create(int id) {
//	ColorLiveEditPopup* ret = new ColorLiveEditPopup();
//	if (ret && ret->init(id)) {
//		ret->autorelease();
//		return ret;
//	}
//	CC_SAFE_DELETE(ret);
//	return nullptr;
//}
//
//bool ColorLiveEditPopup::init(int id) {
//	if (!CCLayer::init()) return false;
//
//	m_colorAction = gd::GameManager::sharedState()->getLevelEditorLayer()->m_levelSettings->m_effectManager->getColorAction(id);
//
//	auto colorPicker = extension::CCControlColourPicker::colourPicker();
//	colorPicker->setColorValue(m_colorAction->m_fromColor);
//	colorPicker->setDelegate(this);
//	this->addChild(colorPicker);
//
//	return true;
//}
//
//void ColorLiveEditPopup::colorValueChanged(ccColor3B value) {
//	m_colorAction->m_fromColor = value;
//	std::cout << value.r << "\n";
//	std::cout << value.g << "\n";
//	std::cout << value.b << "\n";
//}