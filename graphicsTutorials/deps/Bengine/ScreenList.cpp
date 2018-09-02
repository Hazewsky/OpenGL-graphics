//#include "ScreenList.h"
//#include "IGameScreen.h"
//namespace Bengine{
//
//	Bengine::ScreenList::ScreenList(IMainGame* game) :
//	m_game(game){
//
//	}
//
//	ScreenList::~ScreenList(){
//		destroy();
//	}
//	//�����-��������. ���������� ������������ ����� �������� � ���-�� ���
//	//�� �� ������ ������ ������ �� ���������, �� � ���������� ��������� �����
//	IGameScreen* ScreenList::moveNext(){
//
//		IGameScreen* currentScreen = getCurrent();
//		if (currentScreen->getNextScreenIndex() != SCREEN_INDEX_NO_SCREEN){
//			m_currentScreenIndex = currentScreen->getNextScreenIndex();
//		}
//		//���� � ����������, ������ ��� �� �����
//		return getCurrent();
//	}
//
//	IGameScreen* ScreenList::movePrevious(){
//		IGameScreen* currentScreen = getCurrent();
//		if (currentScreen->getPreviousScreenIndex() != SCREEN_INDEX_NO_SCREEN){
//			m_currentScreenIndex = currentScreen->getPreviousScreenIndex();
//		}
//		//���� � ����������, ������ ��� �� �����
//		return getCurrent();
//	}
//
//	//������ �� �������� �����
//	void ScreenList::setScreen(int nextScreen){
//		m_currentScreenIndex = nextScreen;
//
//	}
//
//	void ScreenList::addScreen(IGameScreen* newScreen){
//		//���������� ��� ���������� ��� ������
//		newScreen->m_ScreenIndex = m_screens.size();
//		m_screens.push_back(newScreen);
//		//������ ���
//		newScreen->build();
//		//������ �� ����� ������ ��������, ����
//		newScreen->setParentGame(m_game);
//	}
//
//	void ScreenList::destroy(){
//		for (auto& it : m_screens){
//			it->destroy();
//			
//		}
//		m_screens.resize(0);
//		m_currentScreenIndex = SCREEN_INDEX_NO_SCREEN;
//	}
//
//	
//	IGameScreen* ScreenList::getCurrent(){
//		if (m_currentScreenIndex == SCREEN_INDEX_NO_SCREEN) return nullptr;
//		return m_screens[m_currentScreenIndex];
//	}
//
//
//}

#include "ScreenList.h"

#include "IGameScreen.h"

namespace Bengine {

	ScreenList::ScreenList(IMainGame* game) :
		m_game(game) {
		// Empty
	}

	ScreenList::~ScreenList() {
		destroy();
	}

	IGameScreen* Bengine::ScreenList::moveNext() {
		IGameScreen* currentScreen = getCurrent();
		if (currentScreen->getNextScreenIndex() != SCREEN_INDEX_NO_SCREEN) {
			m_currentScreenIndex = currentScreen->getNextScreenIndex();
		}
		return getCurrent();
	}

	IGameScreen* Bengine::ScreenList::movePrevious() {
		IGameScreen* currentScreen = getCurrent();
		if (currentScreen->getPreviousScreenIndex() != SCREEN_INDEX_NO_SCREEN) {
			m_currentScreenIndex = currentScreen->getPreviousScreenIndex();
		}
		return getCurrent();
	}

	void Bengine::ScreenList::setScreen(int nextScreen) {
		m_currentScreenIndex = nextScreen;
	}

	void Bengine::ScreenList::addScreen(IGameScreen* newScreen) {
		newScreen->m_screenIndex = m_screens.size();
		m_screens.push_back(newScreen);
		newScreen->build();
		newScreen->setParentGame(m_game);
	}

	void Bengine::ScreenList::destroy() {
		for (auto&t: m_screens){
			t->destroy();
			
		}
		m_screens.resize(0);
		m_currentScreenIndex = SCREEN_INDEX_NO_SCREEN;
	}

	IGameScreen* ScreenList::getCurrent() {
		if (m_currentScreenIndex == SCREEN_INDEX_NO_SCREEN) return nullptr;
		return m_screens[m_currentScreenIndex];
	}

}