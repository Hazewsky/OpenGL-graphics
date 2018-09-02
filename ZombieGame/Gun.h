#pragma once
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <Bengine/AudioEngine.h>

#include "Bullet.h"
class Gun
{
public:
	Gun(std::string name, int fireRate, int bulletsPerShot,
		float spread, float bulletSpeed,float bulletDamage,
		Bengine::SoundEffect fireEffect);
	~Gun();

	
	void update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>&bullets, float deltaTime);
private:

	Bengine::SoundEffect z_fireEffect;
	void fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet>&bullets);
	std::string _name;
	int _fireRate;//�������� ��������  � ��������� ������
	int _bulletsPerShot; //������� ���� �������� �� �������
	float _spread; //��������
	float  _bulletSpeed; 
	float _bulletDamage;
	float _frameCounter;

};

