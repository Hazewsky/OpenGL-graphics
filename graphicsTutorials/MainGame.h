#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <Bengine/Sprite.h>
#include <Bengine/GLSLProgram.h>
#include <Bengine/GLTexture.h>
#include <vector>
#include <Bengine/Window.h>
#include <Bengine/Camera2D.h>
#include <Bengine/SpriteBatch.h>
#include <Bengine/InputManager.h>
#include <Bengine/Timing.h>
#include "Bullet.h"
enum class GameState{PLAY,EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();
	
private:
	//private with _

	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();

	Bengine::Window _window;
	int _screenWidth;
	int _screenHeight;
	//�������� �� ����, ��� �������� ���������
	GameState _gameState;
	//std::vector<Bengine::Sprite*> _sprites;//allowes multiple sprites
	//�����
	Bengine::GLSLProgram _colorProgram;
	//������
	Bengine::Camera2D _camera;
	//������������� ���������
	Bengine::SpriteBatch _spriteBatch;

	//���������� �������
	Bengine::InputManager _inputManager;

	//������� ���
	Bengine::FpsLimiter _fpsLimiter;
	//������, ����� ���� ������ �����������, � ����� ��� ������ ����������� � gameLoop
	std::vector<Bullet> _bullets;
	//����� ���� ��� ���. ������ � ����, � � ������� ����� �� ���������� ����� ��������. ����
	float _maxFPS;
	float _fps;
	//������
	float _time;
	
};

