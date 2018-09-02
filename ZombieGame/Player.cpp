#include "Player.h"
#include <SDL/SDL.h>

//� ��� ��� ������ � agent.h, �� ����� ����� ��� ��� ������������
#include <Bengine/ResourceManager.h>
Player::Player():
_currentGunIndex(-1)//�� ������ ������
{
	//EMPTY
}


Player::~Player()
{
	//EMPTY
}

void Player::init(float speed, glm::vec2 pos, Bengine::InputManager * inputManager, Bengine::Camera2D *camera, std::vector<Bullet>*bullets){
	_speed = speed;
	_position = pos;
	_inputManager = inputManager;
	_camera = camera;
	_color = Bengine::Color(255, 255, 255, 255);
	_bullets = bullets;
	_health = 150;
	z_textureID = Bengine::ResourceManager::getTexture("Textures/player.png").id;
}


void Player::addGun(Gun* gun){
	//��������� ������ � ���������
	_guns.push_back(gun);
	//���� �� ����������� �� ������ ������, ���������
	if (_currentGunIndex == -1){
		_currentGunIndex = 0;
	}
}
void Player::update(const std::vector<std::string> &levelData,
		std::vector <Human*> &humans,
		std::vector<Zombie*> &zombies,
		float deltaTime){
	if (_inputManager->isKeyDown(SDLK_w)){
		_position.y += _speed * deltaTime;
	}
	else if (_inputManager->isKeyDown(SDLK_s)){
		_position.y -= _speed * deltaTime;
	}
	if (_inputManager->isKeyDown(SDLK_a)){
		_position.x -= _speed * deltaTime;
	}
	if (_inputManager->isKeyDown(SDLK_d)){
		_position.x += _speed * deltaTime;
	}
	if (_inputManager->isKeyPressed(SDLK_z)){
		_speed++;
	}
	if(_inputManager->isKeyPressed(SDLK_x)){
		if (_speed >= 1){
			_speed--;
		}
		else{
			_speed = 1;
		}
	}

	if (_inputManager->isKeyPressed(SDLK_1) && _guns.size() >= 0){
		_currentGunIndex = 0;
	}

	else if (_inputManager->isKeyPressed(SDLK_2) && _guns.size() >= 1){
		_currentGunIndex = 1;
	}

	else if (_inputManager->isKeyPressed(SDLK_3) && _guns.size() >= 2){
		_currentGunIndex = 2;
	}
	//�������� ���������� ����
	glm::vec2 mouseCoords = _inputManager->getMouseCoords();
	//������� ��� � ������� ����������� ��� ����
	mouseCoords = _camera->convertScreenToWorld(mouseCoords);
	//������� ��������� ������
	glm::vec2 centerPosition = _position + glm::vec2(AGENT_RADIUS);
	//���� ����������� � �����
	z_direction = glm::normalize(mouseCoords - centerPosition);
	if (_currentGunIndex != -1){
		
		
		
		_guns[_currentGunIndex]->update(_inputManager->isKeyDown(SDL_BUTTON_LEFT),
			/*�������� �� ������ ����*/		centerPosition,
											z_direction,
											//��������������, ��� �� ��� ������� ������ �� ����� �������, �� ���������
											*_bullets, deltaTime);
	}
	collideWithLevel(levelData);
}