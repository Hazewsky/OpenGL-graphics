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
	//запущена ли игра, или подобные состо€ни€
	GameState _gameState;
	//std::vector<Bengine::Sprite*> _sprites;//allowes multiple sprites
	//цвета
	Bengine::GLSLProgram _colorProgram;
	//камера
	Bengine::Camera2D _camera;
	//множественна€ отрисовка
	Bengine::SpriteBatch _spriteBatch;

	//обработчик нажатий
	Bengine::InputManager _inputManager;

	//лимитер фпс
	Bengine::FpsLimiter _fpsLimiter;
	//удобно, новые пули просто добавл€ютс€, а потом все вместе обновл€ютс€ в gameLoop
	std::vector<Bullet> _bullets;
	//Ќахер надо мах фпс. ”брать к ху€м, а в функции сразу же передавать нужно значение. »ћ’ќ
	float _maxFPS;
	float _fps;
	//таймер
	float _time;
	
};

