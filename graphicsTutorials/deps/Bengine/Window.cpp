#include "Window.h"
#include "BengineErrors.h"
namespace Bengine{

	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags){

		Uint32 flags = SDL_WINDOW_OPENGL;
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		//Если указан инвиз, добавляет еще и хидден
		if (currentFlags &INVISIBLE){
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN){
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & BORDERLESS){
			flags |= SDL_WINDOW_BORDERLESS;
		}
		//открытие окна. Название, распол. гориз, верт. ширина, высота и тип
		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
		if (_sdlWindow == nullptr){
			fatalError("SDL Window could not be created!");
		}

		SDL_GLContext  glContex = SDL_GL_CreateContext(_sdlWindow);//здесь локально хранятся все объекты
		if (glContex == nullptr){
			fatalError("SDL_GL context could not be created!");
		}
		// glewExperimental = true; в случае странных крашей
		GLenum error = glewInit(); //включает расширения extensions
		if (error != GLEW_OK){//OK==0
			fatalError("Could not initialize glew");
		}
		//показывает версию openGL
		std::printf("*** OpenGL version: %s ***\n", glGetString(GL_VERSION));

		//ставит цвет фона
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		//Пилим Vsync. Оно пытается синхронизировать ФПС с частотой кадров монитора
		//Без него делает т.н. screen tearing. Накладывание 2+ кадров одновременно
		SDL_GL_SetSwapInterval(0);

		//Включить alpha blend ( для прозрачности)
		glEnable(GL_BLEND);
		//Теперь тип блендинга
		//первый фактор источника, второй фактор назначения
		//фактор источника - как мы хотим чтобы альфа повлияла на исходный цвет. мультиплицирует этот фактор, когда будут расчеты цвета
		//там где альфа 1 умножит на цвет и получит цвет. Там же где прозрачный фон( альфа 0) цвет умножится на 0
		//фактор назначение - какую часть цвета на экране мы используем
		//Там где альфа 1 мы должны игнорировать текстуру под нужной нам(голубой фон), там же где альфа(0), мы хотим чтобы
		//фон отображался
		//GL_ONE_MINUS_SRC_ALPHA == 1 - GL_SRC_ALPHA
		//Там где альфа 1 = 1-1 = 0, умножение цвета на 0, т.е. не будет его видно.
		//1-0 = 1, голубой фон будет
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}
	void Window::swapBuffer(){
		SDL_GL_SwapWindow(_sdlWindow);
	}
}