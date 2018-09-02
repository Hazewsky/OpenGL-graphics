#include "MainGame.h"

#include <Bengine/Bengine.h>
#include <SDL/SDL.h>
#include <iostream>
#include <Bengine/Timing.h>
#include <random>
#include <ctime>
#include <Bengine/BengineErrors.h>
#include <Bengine/ResourceManager.h>
#include <glm/gtx/rotate_vector.hpp>
#include "Zombie.h"
#include "Gun.h"
#include <algorithm>
const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;
const float DEG_TO_RAD = M_PI / 180.0f;
MainGame::MainGame() :
_screenWidth(900),
_screenHeight(600),
_gameState(GameState::PLAY),
_fps(0),
_player(nullptr),
_numHumansKilled(0),
_numZombiesKilled(0)
{
}


MainGame::~MainGame()
{
	//������������ ������ ���������� �� ������
	for (auto & lev : _levels){
		delete lev;
	}
	for (auto &hu : _humans){
		delete hu;
	}
	for (auto &zo : _zombies){
		delete zo;
	}
	delete[] _player;
	
}

void MainGame::run(){
	//������ ����� �������������� �������. �������� ������, �.�.�
	initSystems();
	//init level
	initLevel();

	//������� ������
	Bengine::Music music =  z_audioEngine.loadMusic("Sound/XYZ.ogg");
	music.play(-1);
	//� ��������� � ����� ���������
	gameLoop();
	
}
/*
��� ��� ������. ��� ������� ����� �������� � �������� ��������� � ��
void updateBloodParticle(Bengine::Particle2D& particle, float deltaTime){
	particle.p_position += particle.p_velocity * deltaTime;
	//������ ����������
	particle.p_color.a = (GLubyte(particle.p_life * 255.0f));
} */


	//�������������� ���� ���������
void MainGame::initSystems(){
	//������������� ������
	Bengine::init();
	//������������� �����( ������ ����� ����. SDL!!)
	z_audioEngine.init();
	//�������� ����
	_window.create("ZombieGame", _screenWidth, _screenHeight,0);
	//���� ����.����� ������ �������� ���-��
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	//������������� ��������
	initShaders();
	
	_agentSpriteBatch.init();///< ������ �������
	_hudSpriteBatch.init();///<������ �������

	//������������� ������� ������ ����� ��������������� SDL � �������� � OpenGL, ����� ������
	//���� � ����� � ����������(����� � ������, ������������ scale'�� ��� ���������)! ��������, �� ��������� ������ ��� �� 4092 �� �������
	//�� ��� ����� ��������� � ������. �������, ���� ���� ������� ������
	//first printable char & last printable char ��������� �������� � ASCII(�� ��������� 32,126)
	//cs,ce - ���� ��������, �� �� ������ �������� �� ����� ���������� (32-126) � ��� �� ���������� ��� ��� �������� �� ���������
	_spriteFont = new Bengine::SpriteFont("Fonts/horrendo.ttf", 64);
	//������������� ������ � �������� ������
	_camera.init(_screenWidth,_screenHeight);
	_hudCamera.init(_screenWidth, _screenHeight);
	//_hudCamera.setPosition(glm::vec2(_screenWidth/2, _screenHeight/2));

	int a = 100;
	//������������� ������
	p_bloodParticleBatch = new Bengine::ParticleBatch2D;
	p_bloodParticleBatch->init(1000,
								0.05f,
								Bengine::ResourceManager::getTexture("Textures/particle.png"), 
								//updateBloodParticle
								//������. ����� �� ������� ����� �� �������� �������, ����� � ��������� � ��.�.
								//������� ������� ����� ���
								//������ ���������� ������ ������ ��� �� �� ���������� ������� ������� ����������
								//= ��� ������������� �� ��������
								// & �� ������(� ������ � � ����� �����, ��� ��� �������� ����������(����� not allocate with memory)
								[=](Bengine::Particle2D& particle, float deltaTime) {
		particle.p_position += particle.p_velocity * deltaTime;
		//������ ����������
		particle.p_color.a = (GLubyte(particle.p_life * 255.0f));
	});
	p_particleEngine.addParticleBatch(p_bloodParticleBatch);
}

void MainGame::initLevel(){
	//������� 1
	_levels.push_back(new Level("Levels/level1.txt"));
	//��������� ��� �������� � 1 ������
	_currentLevel = 0;
	_player = new Player();
	_player->init(5.0f, _levels[_currentLevel]->getStartPlayerPos(), &_inputManager, &_camera, &_bullets);
	//�.�. player ��������� humans, �� ����� ������� ��� � ������� humans
	_humans.push_back(_player);

	//�������� �������
	//������ �������, ������� ���
	static std::mt19937 randomEngine;
	//���������� ������
	randomEngine.seed(time(nullptr));
	//�������. ������ �������������
	//-2 ��� �� �� ����� �������� �� ��������
	//������� ������ ���� ����������, ����� ����� �� ��


	std::uniform_int_distribution<int> randX(2, _levels[_currentLevel]->getWidth()-2);
	std::uniform_int_distribution<int> randY(2, _levels[_currentLevel]->getHeight()-2);

	//�������� ���� ��������� �������
	for (int i = 0; i < _levels[_currentLevel]->getNumHumans(); i++){
		//_humans.emplace_back(randX(randomEngine, randY(randomEngine)));
		_humans.push_back(new Human);
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) *TILE_WIDTH);
		_humans.back()->init(HUMAN_SPEED, pos);
	}
	//�������� ������
	const std::vector<glm::vec2> & zombiePositions = _levels[_currentLevel]->getZombieStartPos();
	for (auto & it : zombiePositions){
		_zombies.push_back(new Zombie);
		_zombies.back()->init(HUMAN_SPEED, it);
	}

	//��������� ������ ����� ��
	//���, �������� ��������)������ � ������), ���-�� ��������, �������, ��������, �����
	const float BULLET_SPEED = 20.0f;
	_player->addGun(new Gun("Magnum", 10, 1, 1.0f, BULLET_SPEED, 30.0f,
		z_audioEngine.loadSoundEffect("Sound/shots/pistol.wav")));
	_player->addGun(new Gun("Shotgun",30, 8, 20.0f, BULLET_SPEED, 200.0f,
		z_audioEngine.loadSoundEffect("Sound/shots/shotgun.wav")));
	_player->addGun(new Gun("MP-5", 5, 1, 5.0f, BULLET_SPEED, 20.0f,
		z_audioEngine.loadSoundEffect("Sound/shots/cg1.wav")));

}
	//�������������� �������
void MainGame::initShaders(){
	//����������� ������� ����
	_textureProgram.compileShaders("Shaders/VerticesShader.txt", "Shaders/FragmentShader.txt");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}
	//���� ���������. ����������� ������, ����������. ��������� ���, �������� � �.�
void MainGame::gameLoop(){
	Bengine::FpsLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(60.0f);
	//zoom out by 3 times
	const float CAMERA_SCALE = 1.0f / 3.0f;
	_camera.setScale(CAMERA_SCALE);

	//TIme step. ��� ���� �������� ����������� � ���, ��� ���������� ����������, �������� ��� 60 ���, �� ����� ���� ������, ������.
	//���� ������, ���� ����� �����������
	//������ - ����������
	//��� 120 ��� ���� ����� � 2 ���� �������
	//��� ��� ��� ������� ��� �����, �� ������� �������� ����
	
	const float DESIRED_FPS = 60.0f;
	//������������ ���-�� ������ ������� �� ����� ������������
	const int MAX_PHYSICS_STEPS = 6;
	const float MS_PER_SECOND = 1000;
	//FRAME_TIME ������� ����������� �������� �� 1 ����
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
	//������������ ����������� timestep. 1.0 ������ ��� ���������� � ���������
	const float MAX_DELTA_TIME = 1.0f;
	//�������� ������� frametime
	//SDL_GETTICKS ���������� ������� ������ ����������� � ��� ��� ��� ��� ��� ���������������
	float previousTicks = SDL_GetTicks();
	while (_gameState == GameState::PLAY){
		//����������� ���
		fpsLimiter.begin();
		float newTicks = SDL_GetTicks();
		//������� ����������� ����� ����
		float frameTime = newTicks - previousTicks;
		//���������� ����������
		previousTicks = newTicks;

		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;
		checkVictory();
		_inputManager.update();
		//���������� �����
		processInput();
		//��� ������ ��������� ����, �� ���� ���� ���������.
		//���� � ��� ����� ������ �����, ������� spire loop death
		//�� �������� ������������ ������ � ������ ������, �� �� ��������� � ���������
		//������ ��� ���� ��������� ����� ������
		int i = 0;
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS){
			//���� ����� ��������� �� 120 ���, totalDeltaTime ����� ������(0.5), � ���� ����� �����������

			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			//��������� �������
			updateAgents(deltaTime);
			//��������� ����
			updateBullets(deltaTime);
			//���������� ������
			p_particleEngine.update(totalDeltaTime);
			//��� ��� ���������, ����� ����������� �� totalDeltaTime
			//�����?
			totalDeltaTime -= deltaTime;
			i++;
		}

		
		//��������� ������
		_camera.setPosition(_player->getPosition());
		_camera.update();
		_hudCamera.update();
		drawGame();


		_fps=fpsLimiter.end();
		std::cout << _fps << std::endl;
	}
}

void MainGame::updateAgents(float deltaTime){
	//���������� ���� �����
	for (auto & it : _humans){
		it->update(_levels[_currentLevel]->getLevelData(),
			_humans,
			_zombies, deltaTime);
	}
	//���������� ������
	for (auto & it : _zombies){
		it->update(_levels[_currentLevel]->getLevelData(),
			_humans,
			_zombies, deltaTime);
	}
	//���������� �������� �����
	for (int i = 0; i < _zombies.size(); i++){
		//� ������� �����
		for (int j = i + 1; j < _zombies.size(); j++){
			_zombies[i]->collideWithAgent(_zombies[j]);
		}
		//� ������
		//j=0 �����. ������� ����
		for (int j = 1; j < _humans.size(); j++){
			if (_zombies[i]->collideWithAgent(_humans[j])){
				_zombies.push_back(new Zombie);
				//����� ����� � ��� �� ����� ������� ��� ��� "����������" ���
				_zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());
				//������� ����
				delete _humans[j];
				_humans[j] = _humans.back();
				_humans.pop_back();

			}
		}
		//�������� � �������
		if (_zombies[i]->collideWithAgent(_player)){
			_gameState = GameState::EXIT;
			Bengine::fatalError("YOU, LOOSER!");
			
			}
	}
	//���������� �������� �����
	//������ ������� ����������� �� ������� �� ����� ����������
	/*for (auto& i : _humans){
		for (auto & j)
	}*/
	for (int i = 0; i < _humans.size(); i++){
		//�������� ����� � ������
		//��������� ��������� ����� ������� ���-�� �����. ��� ���������� ��� �����, ������� �����������
		for (int j = i + 1; j < _humans.size(); j++){
			_humans[i]->collideWithAgent(_humans[j]);
		}
	}
	//�� ������ �������� ��������
}

void MainGame::updateBullets(float deltaTime){
	//����������� � ������� � �����
	for (int i = 0; i < _bullets.size();){
		//���� update ���������� true, ���� ������������� �� ������
		if (_bullets[i].update(_levels[_currentLevel]->getLevelData(), deltaTime)){
			//�� ���� ������ delete [], ��� ���� �� � ��� �� New
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		//���� �� ������� ����, ��� �� ���� ����������������, ����� ��������� ���������� ��������
		else{
			i++;
		}
	}

	bool wasBulletRemoved;
//������� � ������ � �������
	for (int i = 0; i < _bullets.size();i++){
		wasBulletRemoved = false;
		
		//��� ����� ������

		for (int j = 0; j < _zombies.size();){
			//�������� ������������
			if (_bullets[i].collideWithAgent(_zombies[j])){
				//��������� �������
				addBlood(_bullets[i].getPosition(), 7);

				//����� ����� � ����� ���, ���� �� == 0
				//���� ����� ����
				if (_zombies[j]->applyDamage(_bullets[i].getDamage())){
					//���� ����� �����, ������� ���
					delete _zombies[j];
					_zombies[j] = _zombies.back();
					_zombies.pop_back();
					_numZombiesKilled++;
				}
				else{
					j++;
				}
				//������� ����
				_bullets[i] = _bullets.back();
				_bullets.pop_back();

				//continue ������� ��� ���������
				//break ��������� �������� ����� �����
				//������?
				//������ ��� ���� ������, � ���������� ���� ��� ������.
				//��� ��� �� �����, ������� ��������� ��������, ����� ��.�
				//�.�. ���� �������� ������, � �� ������� ����, ���� ��������� �����
				//� ����������, ��� �� �������� ����.����� � ��� ����� �� ���� ���� ������, �������
				wasBulletRemoved = true;
				i--;
				break;
			}
			else{
				j++;
			}
		}
		//��������� ����. ���� �� ����� ����, ������ ���� �� ���� �������. ������
		//��� ������ �����
		//���� ���� �� ������
		if (wasBulletRemoved == false){
			for (int j = 1; j < _humans.size();){
				//�������� ������������
				if (_bullets[i].collideWithAgent(_humans[j])){
					//������ �������
					addBlood(_bullets[i].getPosition(), 5);

					//����� ����� � ����� ���, ���� �� == 0
					//���� ����� ����
					if (_humans[j]->applyDamage(_bullets[i].getDamage())){
						//���� ����� �����, ������� ���
						delete _humans[j];
						_humans[j] = _humans.back();
						_humans.pop_back();
						_numHumansKilled++;
					}
					else{
						j++;
					}
					//������� ����
					_bullets[i] = _bullets.back();
					_bullets.pop_back();

					//continue ������� ��� ���������
					//break ��������� �������� ����� �����
					//������?
					//������ ��� ���� ������, � ���������� ���� ��� ������.
					//��� ��� �� �����, ������� ��������� ��������, ����� ��.�
					//�.�. ���� �������� ������, � �� ������� ����, ���� ��������� �����
					//� ����������, ��� �� �������� ����.����� � ��� ����� �� ���� ���� ������, �������
					wasBulletRemoved = true;
					i--;
					break;
				}
				else{
					j++;
				}
			}
		}
		
	}
}
//������� ������
void MainGame::checkVictory(){
	//TODO: ��������� ���������� �������
	//_currentLevel++;
	//initLevel(...);
	//���� ��� ����� ������, �� ��������
	if (_zombies.empty()){
		std::printf("****You win!****\n You killed %d humans and %d zombies. There are %d/%d humans remaining",
			_numHumansKilled, _numZombiesKilled, _humans.size() - 1, _levels[_currentLevel]->getNumHumans());
		Bengine::fatalError("");
	}
}
	//���������� �������
void MainGame::processInput(){
	SDL_Event evnt;
	//����� ��������� ���� ���� �������� ��� ���������
	while (SDL_PollEvent(&evnt)){
		switch (evnt.type){
		case SDL_QUIT:
			//����� �����
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}
}

	//������ ����
void MainGame::drawGame(){
	//����� ������� ������� 1.0
	glClearDepth(1.0f);
	//������� ������ ����� � �������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_textureProgram.use();
	//����� ��� ���������
	//������� ��� ����� ������������ Texture unit 0
	glActiveTexture(GL_TEXTURE0);
	//��������� ��� mySampler ���� ����� ���� ����
	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	//�������� ������� ������
	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	//�������� uniform �� ������
	GLint pUniform = _textureProgram.getUniformLocation("P");
	//&projectionMatrix[0][0] ����� ��� ������
	glUniformMatrix4fv(pUniform, 1,GL_FALSE, &projectionMatrix[0][0]);


	//������ �������
	_levels[_currentLevel]->draw();
	//������ ��������� �������
	_agentSpriteBatch.begin();

	//������ const ���� ����� ������ ������� �������
	const glm::vec2 agentDims(AGENT_RADIUS * 2.0f);
//	������ �����

	for (auto & it : _humans){
		if (_camera.isBoxInView(it->getPosition(), agentDims)){
			it->draw(_agentSpriteBatch);
		}
	}
	//	������ ������
	for (auto & it : _zombies){
		if (_camera.isBoxInView(it->getPosition(), agentDims)){
			it->draw(_agentSpriteBatch);
		}
	}
	//������ ����
	for (auto &it : _bullets){
		it.draw(_agentSpriteBatch);
	}

	_agentSpriteBatch.end();
	_agentSpriteBatch.renderBatch();
	//��������� ������
	p_particleEngine.draw(&_agentSpriteBatch);
	//��������� ���������
	drawHud();
	_textureProgram.unuse();
	//������ ������(� ��� �� 2) � ������������ ��� �� �����
	_window.swapBuffer();
}

void MainGame::drawHud(){
	//����� �������� ������ ��� �����
	char buffer[256];

	glm::mat4 projectionMatrix = _hudCamera.getCameraMatrix();
	//�������� uniform �� ������
	GLint pUniform = _textureProgram.getUniformLocation("P");
	//&projectionMatrix[0][0] ����� ��� ������
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	_hudSpriteBatch.begin();
	//������ �������� �� �����, �������� � buffer
	sprintf_s(buffer, "Humber of Humans alive %d", _humans.size());
	//���� buffer ��� �� string, �� ���� ���� ����� ��� � c_str()
	//justification -������������. Left ������
	_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(50, -200), glm::vec2(0.5f), 0.0f,
		Bengine::Color(0, 0, 0, 255), Bengine::Justification::MIDDLE);
	sprintf_s(buffer, "Humber of Zombies alive %d", _zombies.size());
	_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(50, -232), glm::vec2(0.5f), 0.0f,
		Bengine::Color(0, 0, 0, 255), Bengine::Justification::MIDDLE);
	_hudSpriteBatch.end();
	_hudSpriteBatch.renderBatch();
}

void MainGame::addBlood(const glm::vec2& position, int numParticles){
	//������ �������, ������� ���
	static std::mt19937 randomEngine(time(nullptr));
//�������
	static std::uniform_real_distribution<float> randAngle(0.0f * DEG_TO_RAD, 360.0f * DEG_TO_RAD);

	glm::vec2 vel(2.0f, 0.0f);
	for (int i = 0; i < numParticles; i++){
		p_bloodParticleBatch->addParticle(position,
											glm::rotate(vel, randAngle(randomEngine)),
											Bengine::Color(255, 0, 0, 255),
											20.0f);
	}
}