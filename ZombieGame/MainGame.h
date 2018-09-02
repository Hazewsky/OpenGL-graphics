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

	//��������� ����
	void run();
private:
	//�������������� ���� ���������
	void initSystems();
	//����. ������� � ������������� ���
	void initLevel();
	//�������������� �������
	void initShaders();
	//���� ���������. ����������� ������, ����������. ��������� ���, �������� � �.�
	void gameLoop();
	//��������� ���� �������
	void updateAgents(float deltaTime);

	//��������� ����
	void updateBullets(float deltaTime);
	//������� ������
	void checkVictory();
	//���������� �������
	void processInput();

	//������ ����
	void drawGame();

	void drawHud();
	//��������� ����� � ������ ������
	void addBlood(const glm::vec2& position, int numParticles);
	//������ _���   - ��� �����?
	//���������� ����������� ���������� � ����� ��������������. ������������, ��� �� �� ����������� �������� ����
	//� ��� ������� �� ������
	//�� ����� �� ���������
	Bengine::Window _window; //������� ����

	Bengine::GLSLProgram _textureProgram; //��������� ��������

	Bengine::InputManager _inputManager; //���������� �������

	Bengine::Camera2D _camera; //<������� ������
	Bengine::Camera2D _hudCamera; //< ����� ��� ��������, ����� ��� �� ���� ���������� �� ����� ����� �� �����

	Bengine::SpriteBatch _agentSpriteBatch; //������ ���� �������

	Bengine::SpriteBatch _hudSpriteBatch; ///< ������ �������?
	//������ ������
	Bengine::ParticleEngine2D p_particleEngine;
	//���� ��� �����
	Bengine::ParticleBatch2D* p_bloodParticleBatch;
	//����  ����� ����� ������� ������ �� ����� ��������
	std::vector<Level *> _levels;
	//������� ���� ������������� � ������, � ����� � ����. �����
	int _screenWidth = 900;
	int _screenHeight = 600;
	float _fps;
	int _currentLevel;
	Player* _player;
	std::vector<Human*> _humans;//������ ���� �����
	std::vector<Zombie*> _zombies;//������ ���� �����
	std::vector<Bullet> _bullets;

	int _numHumansKilled; //���� ������ �������
	int _numZombiesKilled; //����� ������ �������

	Bengine::SpriteFont* _spriteFont;///< ������� � ���������� ��������

	Bengine::AudioEngine z_audioEngine;
	GameState _gameState;
};

