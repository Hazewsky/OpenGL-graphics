//#pragma once
//#include <vector>
//namespace Bengine{
//	//можем делать только если указатели
//	class IMainGame;
//	class IGameScreen;
//	class ScreenList
//	{
//
//	public:
//		ScreenList(IMainGame* game);
//		~ScreenList();
//		//Класс-помощник. Перемогает перемещаться между экранами и что-то еще
//		//мы не только меняем индекс на следующий, но и возвращаем следующий экран
//		IGameScreen* moveNext();
//		IGameScreen* movePrevious();
//		//Меняет на активный экран
//		void setScreen(int nextScreen);
//
//		void addScreen(IGameScreen* newScreen);
//
//		void destroy();
//
//		IGameScreen* getCurrent();
//	protected:
//		IMainGame* m_game = nullptr;
//		//лучше не надо делается указатели на элемент вектора
//		std::vector<IGameScreen*> m_screens;
//		int m_currentScreenIndex = -1;
//
//
//	};
//
//}

#pragma once

#include <vector>

namespace Bengine {

	class IMainGame;
	class IGameScreen;

	class ScreenList {
	public:
		ScreenList(IMainGame* game);
		~ScreenList();

		IGameScreen* moveNext();
		IGameScreen* movePrevious();

		void setScreen(int nextScreen);
		void addScreen(IGameScreen* newScreen);

		void destroy();

		IGameScreen* getCurrent();

	protected:
		IMainGame* m_game = nullptr;
		std::vector<IGameScreen*> m_screens;
		int m_currentScreenIndex = -1;
	};

}