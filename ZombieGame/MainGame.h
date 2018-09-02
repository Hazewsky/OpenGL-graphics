#pragma once

#include <Bengine/GLSLProgram.h>
#include <Bengine/Camera2D.h>
#include <Bengine/Window.h>
#include <Bengine/InputManager.h>
#include <Bengine/SpriteFont.h>
#include <Bengine/SpriteBatch.h>
#include <Bengine/AudioEngine.h>
#include <Bengine/ParticleEngine2D.h>
#include <Bengine/ParticleBatch2D.h>
#include "Player.h"
#include "Level.h"
#include "Bullet.h"
class Zombie;

enum class GameState{
	PLAY,
	EXIT
};
class MainGame
{
public:
	MainGame();
	~MainGame();

	//запускает игру
	void run();
private:
	//инициализирует ядро программы
	void initSystems();
	//иниц. уровень и устанавливает все
	void initLevel();
	//инициализирует шейдеры
	void initShaders();
	//цикл программы. возможность играть, собственно. отрисовка там, движения и т.д
	void gameLoop();
	//обновляет всех агентов
	void updateAgents(float deltaTime);

	//обновляет пули
	void updateBullets(float deltaTime);
	//условия победы
	void checkVictory();
	//обработчик нажатий
	void processInput();

	//рендер игры
	void drawGame();

	void drawHud();
	//добавляет кровь в движок частиц
	void addBlood(const glm::vec2& position, int numParticles);
	//почему _вар   - это плохо?
	//компилятор резервирует переменные с таким подчеркиванием. маловероятно, что он бы использовал подобный бред
	//и это вызвало бы пиздец
	//но лучше не рисковать
	Bengine::Window _window; //игровое окно

	Bengine::GLSLProgram _textureProgram; //программа шейдеров

	Bengine::InputManager _inputManager; //обработчик событий

	Bengine::Camera2D _camera; //<главная камера
	Bengine::Camera2D _hudCamera; //< камеа для надписей, чтобы они не были закреплены на одной точке на карте

	Bengine::SpriteBatch _agentSpriteBatch; //рисует всех агентов

	Bengine::SpriteBatch _hudSpriteBatch; ///< рисует надписи?
	//движок частиц
	Bengine::ParticleEngine2D p_particleEngine;
	//батч для крови
	Bengine::ParticleBatch2D* p_bloodParticleBatch;
	//если  будет много уровней вектор со всеми уровнями
	std::vector<Level *> _levels;
	//сначала идет инициализация в хедере, а потом в иниц. листе
	int _screenWidth = 900;
	int _screenHeight = 600;
	float _fps;
	int _currentLevel;
	Player* _player;
	std::vector<Human*> _humans;//вектор всех людей
	std::vector<Zombie*> _zombies;//вектор всех зомби
	std::vector<Bullet> _bullets;

	int _numHumansKilled; //люди убитые игроком
	int _numZombiesKilled; //зомби убитые игроком

	Bengine::SpriteFont* _spriteFont;///< надписи с кастомными шрифтами

	Bengine::AudioEngine z_audioEngine;
	GameState _gameState;
};

