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
	//���� �� ����, ����� �������� ��������� � ����
	if (closestHuman != nullptr){
		//������� ������ - ������� �����
		//vector A - vector B = arrow pointing to vector A. Pointing torwards closest human
		//�����������
		z_direction = glm::normalize(closestHuman->getPosition() - _position);
		_position += z_direction * _speed * deltaTime;
	}


}

Human* Zombie::getNearestHuman(std::vector <Human*> &humans){
	//���� ������ ���, ������ ��������� �� ��������!
	Human* closestHuman = nullptr;
	//��������� �� ���������� ��������
	float smallestDistance = 9999999.0f;

	for (auto& it : humans){
		glm::vec2 distVec = it->getPosition()-_position;
		float distance = glm::length(distVec);
		//���� ����������
		if (distance < smallestDistance){
			smallestDistance = distance;
			closestHuman = it;
		}
	}
	return closestHuman;
}