#include "Human.h"

#include <ctime>
#include <random>
//позвол€ет крутить вектора
#include <glm/gtx/rotate_vector.hpp>
const float DEG_TO_RAD = 3.14159265359f/180.0f;
Human::Human() :_frames(0)
{
}


Human::~Human()
{
}


void Human::init(float speed, glm::vec2 pos){
	//движок рандома, хороший тип
	//изложенный ниже код решаетс€ сбором в конструкторе
	static std::mt19937 randomEngine(time(nullptr));
	/*
	//нужно сидить только 1 раз
	static bool isSeeded = false;
	//нужен только один раз, иначе посто€нно будет одно и то же число
	if (isSeeded = false){
		randomEngine.seed(time(nullptr));
		isSeeded = true;
	}
	*/
	//а можно вот так все решить
	//границы
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);
	//сделать так, чтобы при заражении новый зомби получал хп человека. ¬ инит зомби вставить хп, тут добавить гетер по хп
	//+ рандомизировать или поделить на классы хп людей
	_health = 1;
	//цвет чисто белый чтобы не красило текстуру
	_color = Bengine::Color(255, 255, 255, 255);
	_speed = speed;
	_position = pos;
	z_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	//в случае если нулевое направление будет(сто€ть на месте)
	if (z_direction.length() == 0){
		z_direction = glm::vec2(1.0f, 0.0f);
	}
	//нормализуем. ƒелает unit length
	z_direction = glm::normalize(z_direction);
	z_textureID = Bengine::ResourceManager::getTexture("Textures/human.png").id;
}



void Human::update(const std::vector<std::string> &levelData,
	std::vector <Human*> &humans,
	std::vector<Zombie*> &zombies, float deltaTime){
	
	static std::mt19937 randomEngine(time(nullptr));
	//rotate использует радианы. ¬нимательно. надо переводить из градусов. *PI/180
	static std::uniform_real_distribution<float> randRotate(-40.0f * DEG_TO_RAD, 40.0f* DEG_TO_RAD);
	//без ускорени€
	_position += z_direction * _speed * deltaTime;
	//случайно мен€ет направление каждые 20 кадров
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