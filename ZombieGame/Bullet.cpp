#include "Bullet.h"
#include <Bengine/ResourceManager.h>
#include "Agent.h"
#include "Human.h"
#include "Zombie.h"
#include "Level.h"
#include <iostream>
#include <glm/glm.hpp>
Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed) :
_position(position),
_direction(direction),
_damage(damage),
_speed(speed)
{
}


Bullet::~Bullet()
{
}

bool Bullet::update(const std::vector<std::string> & levelData, float deltaTime){
	_position += _direction *_speed * deltaTime;
	return collideWithWorld(levelData);
}

void Bullet::draw(Bengine::SpriteBatch& spriteBatch){
	glm::vec4 destRect(_position.x - BULLET_RADIUS, _position.y - BULLET_RADIUS, BULLET_RADIUS * 2, BULLET_RADIUS * 2);
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	Bengine::Color color(0, 0, 0,255);
	spriteBatch.draw(destRect, uvRect, Bengine::ResourceManager::getTexture("Textures/bull.png").id,0.0f,color);
}
//сферическая коллизия
bool Bullet::collideWithAgent(Agent * agent){
	const float MIN_DISTANCE = BULLET_RADIUS + AGENT_RADIUS;
	//позиция пули
	glm::vec2 centerPosA = _position;
	//позиция цели
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = (centerPosA - centerPosB);

	//длина вектора между двумя центрами
	float distance = glm::length(distVec);
	//глубина коллизии
	float collisionDepth = MIN_DISTANCE - distance;
	if (collisionDepth > 0){
		return true;
	}
	return false;



}

bool Bullet::collideWithWorld(const std::vector<std::string> & levelData){
	glm::ivec2 gridPosition;
	gridPosition.x = floor(_position.x / (float)TILE_WIDTH);
	gridPosition.y = floor(_position.y / (float)TILE_WIDTH);

	if (gridPosition.x < 0 || gridPosition.x >= levelData[0].length() ||
		gridPosition.y < 0 || gridPosition.y >= levelData.size()){
		return true;
	}
	//если тайл не пустой, коллайд
	return (levelData[gridPosition.y][gridPosition.x] != '.');
}