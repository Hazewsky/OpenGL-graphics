#include "Zombie.h"
#include "Human.h"
Zombie::Zombie()
{
}


Zombie::~Zombie()
{
}
void Zombie::init(float speed, glm::vec2 pos){
	_health = 150;
	_speed = speed;
	_position = pos;
	_color = Bengine::Color(255, 255, 255, 255);
	z_textureID = Bengine::ResourceManager::getTexture("Textures/zombie.png").id;
}
void Zombie::update(const std::vector<std::string> &levelData,
	std::vector <Human*> &humans,
	std::vector<Zombie*> &zombies,
	float deltaTime){

	collideWithLevel(levelData);
	Human * closestHuman = getNearestHuman(humans);
	//если он есть, зомби начинает двигаться к нему
	if (closestHuman != nullptr){
		//позиция жертвы - позиция зомби
		//vector A - vector B = arrow pointing to vector A. Pointing torwards closest human
		//нормализуем
		z_direction = glm::normalize(closestHuman->getPosition() - _position);
		_position += z_direction * _speed * deltaTime;
	}


}

Human* Zombie::getNearestHuman(std::vector <Human*> &humans){
	//если вернет его, значит человеков не осталось!
	Human* closestHuman = nullptr;
	//дистанция до ближайшего чебурека
	float smallestDistance = 9999999.0f;

	for (auto& it : humans){
		glm::vec2 distVec = it->getPosition()-_position;
		float distance = glm::length(distVec);
		//ищет ближайшего
		if (distance < smallestDistance){
			smallestDistance = distance;
			closestHuman = it;
		}
	}
	return closestHuman;
}