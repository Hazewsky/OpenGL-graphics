#include "Human.h"

#include <ctime>
#include <random>
//��������� ������� �������
#include <glm/gtx/rotate_vector.hpp>
const float DEG_TO_RAD = 3.14159265359f/180.0f;
Human::Human() :_frames(0)
{
}


Human::~Human()
{
}


void Human::init(float speed, glm::vec2 pos){
	//������ �������, ������� ���
	//���������� ���� ��� �������� ������ � ������������
	static std::mt19937 randomEngine(time(nullptr));
	/*
	//����� ������ ������ 1 ���
	static bool isSeeded = false;
	//����� ������ ���� ���, ����� ��������� ����� ���� � �� �� �����
	if (isSeeded = false){
		randomEngine.seed(time(nullptr));
		isSeeded = true;
	}
	*/
	//� ����� ��� ��� ��� ������
	//�������
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);
	//������� ���, ����� ��� ��������� ����� ����� ������� �� ��������. � ���� ����� �������� ��, ��� �������� ����� �� ��
	//+ ��������������� ��� �������� �� ������ �� �����
	_health = 1;
	//���� ����� ����� ����� �� ������� ��������
	_color = Bengine::Color(255, 255, 255, 255);
	_speed = speed;
	_position = pos;
	z_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	//� ������ ���� ������� ����������� �����(������ �� �����)
	if (z_direction.length() == 0){
		z_direction = glm::vec2(1.0f, 0.0f);
	}
	//�����������. ������ unit length
	z_direction = glm::normalize(z_direction);
	z_textureID = Bengine::ResourceManager::getTexture("Textures/human.png").id;
}



void Human::update(const std::vector<std::string> &levelData,
	std::vector <Human*> &humans,
	std::vector<Zombie*> &zombies, float deltaTime){
	
	static std::mt19937 randomEngine(time(nullptr));
	//rotate ���������� �������. �����������. ���� ���������� �� ��������. *PI/180
	static std::uniform_real_distribution<float> randRotate(-40.0f * DEG_TO_RAD, 40.0f* DEG_TO_RAD);
	//��� ���������
	_position += z_direction * _speed * deltaTime;
	//�������� ������ ����������� ������ 20 ������
	if (_frames == 20){
		z_direction = glm::rotate(z_direction, randRotate(randomEngine));
		_frames = 0;
	}
	else{
		_frames++;
	}
	if (collideWithLevel(levelData)){
		
		z_direction = glm::rotate(z_direction, randRotate(randomEngine));
	}
	


}