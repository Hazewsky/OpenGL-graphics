#include "Gun.h"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>
const float DEG_TO_RAD = 3.14159265359f / 180.0f;
Gun::Gun(std::string name, int fireRate, int bulletsPerShot,
		float spread, float bulletSpeed, float bulletDamage,
		Bengine::SoundEffect fireEffect) :

_name(name),
_fireRate(fireRate),
_bulletsPerShot(bulletsPerShot),
_spread(spread),
_bulletSpeed(bulletSpeed),
_bulletDamage(bulletDamage),
_frameCounter(0),
z_fireEffect(fireEffect)
{
}


Gun::~Gun()
{

}
void Gun::update(bool isMouseDown,const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>&bullets, float deltaTime){
	_frameCounter+= 1.0f * deltaTime;
	//если кнопка нажата и кол-во кадров > скорости стрельбы, в таком случае стреляем
	if (_frameCounter >= _fireRate && isMouseDown){
		fire(direction,position, bullets);
		_frameCounter = 0;
	}
}
void Gun::fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet>&bullets){
	//рандомный разброс, базирующийся на точности
	static std::mt19937 randomEngine(time(nullptr));
	 std::uniform_real_distribution<float> randRotate(-_spread *DEG_TO_RAD, _spread*DEG_TO_RAD);
	 //звук стрельбы
	 z_fireEffect.play();
	 //стрельба в нужном направлении
	for (int i = 0; i < _bulletsPerShot; i++){
		bullets.emplace_back(position, 
							glm::rotate(direction,randRotate(randomEngine)),
							_bulletDamage, 
							_bulletSpeed);
	}
}