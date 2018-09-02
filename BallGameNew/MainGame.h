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
#include "Grid.h"
enum class GameState { RUNNING, EXIT };

const int CELL_SIZE = 12;
class MainGame {
public:
	~MainGame();
    void run();
	
private:
	void init();
	void initRenderers();
    void initBalls();
    void update(float deltaTime);
    void draw();
    void drawHud();
    void processInput();

    int m_screenWidth = 0;
    int m_screenHeight = 0;
	//если количество шаров будет меняться, обязательно Ball*
    std::vector<Ball> m_balls; ///< All the balls
	//уникальный указатель. Только студия 11+. Прелесть в том что он сам освободает память
	std::unique_ptr<Grid> m_grid = nullptr; ///< сетка для spatial partition ля столкновений\

	int m_curerentRenderer = 0;
	//полиморфизм. вектор видов рендеров
	std::vector<BallRenderer*> m_ballRenderer;
    BallController m_ballController; ///< Controls balls
    //BallRenderer m_ballRenderer; ///< Renders balls

    Bengine::Window m_window; ///< The main window
    Bengine::SpriteBatch m_spriteBatch; ///< Renders all the balls
    std::unique_ptr<Bengine::SpriteFont> m_spriteFont; ///< For font rendering
    Bengine::Camera2D m_camera; ///< Renders the scene
    Bengine::InputManager m_inputManager; ///< Handles input
    Bengine::GLSLProgram m_textureProgram; ///< Shader for textures]

    Bengine::FpsLimiter m_fpsLimiter; ///< Limits and calculates fps
    float m_fps = 0.0f;

    GameState m_gameState = GameState::RUNNING; ///< The state of the game
};

