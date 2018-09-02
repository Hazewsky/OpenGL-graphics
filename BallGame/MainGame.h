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
//��������� ����
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

	std::vector<Ball> balls_;///<��� ����

	BallController ballController_;///< ���������� ������
	BallRenderer ballRenderer_;///< ��������� �����

	Bengine::Window window_;///< ������� ����
	Bengine::SpriteBatch spriteBatch_;///< ��������
	std::unique_ptr<Bengine::SpriteFont> spriteFont_; ///< �������
	Bengine::Camera2D camera_;///< ������
	Bengine::InputManager inputManager_;///< ���������� �������
	Bengine::GLSLProgram textureProgram_; ///< �������

	Bengine::FpsLimiter fpsLimiter_; ///< ������������ ���
	float fps_ = 0.0f;

	GameState gameState_ = GameState::RUNNING;///<���� �� ��������� ��������

};

