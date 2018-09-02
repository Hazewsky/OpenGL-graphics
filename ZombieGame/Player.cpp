#include "Player.h"
#include <SDL/SDL.h>

//я это уже запили в agent.h, но пусть будет тут как альтернатива
#include <Bengine/ResourceManager.h>
Player::Player():
_currentGunIndex(-1)//ни одного оружия
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
	//добавляет оружие в инвентарь
	_guns.push_back(gun);
	//если не экипировано ни одного оружия, экипирует
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
	//получаем координаты мыши
	glm::vec2 mouseCoords = _inputManager->getMouseCoords();
	//смотрим где в мировых координатах был клик
	mouseCoords = _camera->convertScreenToWorld(mouseCoords);
	//текущее положение игрока
	glm::vec2 centerPosition = _position + glm::vec2(AGENT_RADIUS);
	//ищем направление к клику
	z_direction = glm::normalize(mouseCoords - centerPosition);
	if (_currentGunIndex != -1){
		
		
		
		_guns[_currentGunIndex]->update(_inputManager->isKeyDown(SDL_BUTTON_LEFT),
			/*Стартует из цетнра тела*/		centerPosition,
											z_direction,
											//разыменовываем, ибо от нас требуют ссылку на класс буллетс, не указатель
											*_bullets, deltaTime);
	}
	collideWithLevel(levelData);
}