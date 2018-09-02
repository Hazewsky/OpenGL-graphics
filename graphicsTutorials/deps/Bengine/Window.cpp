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
		//���� ������ �����, ��������� ��� � ������
		if (currentFlags &INVISIBLE){
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN){
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & BORDERLESS){
			flags |= SDL_WINDOW_BORDERLESS;
		}
		//�������� ����. ��������, ������. �����, ����. ������, ������ � ���
		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
		if (_sdlWindow == nullptr){
			fatalError("SDL Window could not be created!");
		}

		SDL_GLContext  glContex = SDL_GL_CreateContext(_sdlWindow);//����� �������� �������� ��� �������
		if (glContex == nullptr){
			fatalError("SDL_GL context could not be created!");
		}
		// glewExperimental = true; � ������ �������� ������
		GLenum error = glewInit(); //�������� ���������� extensions
		if (error != GLEW_OK){//OK==0
			fatalError("Could not initialize glew");
		}
		//���������� ������ openGL
		std::printf("*** OpenGL version: %s ***\n", glGetString(GL_VERSION));

		//������ ���� ����
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		//����� Vsync. ��� �������� ���������������� ��� � �������� ������ ��������
		//��� ���� ������ �.�. screen tearing. ������������ 2+ ������ ������������
		SDL_GL_SetSwapInterval(0);

		//�������� alpha blend ( ��� ������������)
		glEnable(GL_BLEND);
		//������ ��� ���������
		//������ ������ ���������, ������ ������ ����������
		//������ ��������� - ��� �� ����� ����� ����� �������� �� �������� ����. ��������������� ���� ������, ����� ����� ������� �����
		//��� ��� ����� 1 ������� �� ���� � ������� ����. ��� �� ��� ���������� ���( ����� 0) ���� ��������� �� 0
		//������ ���������� - ����� ����� ����� �� ������ �� ����������
		//��� ��� ����� 1 �� ������ ������������ �������� ��� ������ ���(������� ���), ��� �� ��� �����(0), �� ����� �����
		//��� �����������
		//GL_ONE_MINUS_SRC_ALPHA == 1 - GL_SRC_ALPHA
		//��� ��� ����� 1 = 1-1 = 0, ��������� ����� �� 0, �.�. �� ����� ��� �����.
		//1-0 = 1, ������� ��� �����
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}
	void Window::swapBuffer(){
		SDL_GL_SwapWindow(_sdlWindow);
	}
}