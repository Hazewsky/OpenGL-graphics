#pragma once

#include <Bengine/Camera2D.h>
#include <Bengine/SpriteBatch.h>
#include <Bengine/InputManager.h>
#include <Bengine/Window.h>
#include <Bengine/GLSLProgram.h>
#include <Bengine/Timing.h>
#include <Bengine/SpriteFont.h>
#include <memory>

#include "BallController.h"
#include "BallRenderer.h"
//состояние игры
enum class GameState { RUNNING, EXIT };

class MainGame
{
public:
	void run();
private:
	void init();
	void initBalls();
	void update(float deltaTime);
	void draw();
	void drawHud();
	void processInput();

	int screenWidth_ = 0;
	int screenHeight_ = 0;

	std::vector<Ball> balls_;///<все шары

	BallController ballController_;///< управление шарами
	BallRenderer ballRenderer_;///< отрисовка шаров

	Bengine::Window window_;///< игровое окно
	Bengine::SpriteBatch spriteBatch_;///< текстуры
	std::unique_ptr<Bengine::SpriteFont> spriteFont_; ///< надписи
	Bengine::Camera2D camera_;///< камера
	Bengine::InputManager inputManager_;///< обработчик нажатий
	Bengine::GLSLProgram textureProgram_; ///< шейдеры

	Bengine::FpsLimiter fpsLimiter_; ///< ограничитель фпс
	float fps_ = 0.0f;

	GameState gameState_ = GameState::RUNNING;///<игра по умолчанию запущена

};

