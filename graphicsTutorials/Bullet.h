#pragma once
#include <glm/glm.hpp>
#include <Bengine/SpriteBatch.h>

class Bullet
{
public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime);
	~Bullet();
	//Нам нужен нормальзованный вектор(с длиной 1), для этого надо x координаты и y поделить на длину вектора между 2 точками
	//гипотенузу, то есть. И вот это будет нормализованном, (unit) вектором. в glm есть спец. функция normalize.

	void draw(Bengine::SpriteBatch &spriteBatch);
	//false если пуля еще живет, true если ее _lifeTIme == 0 и ее надо уничтожить
	bool update();

private:
	//как далеко идет за 1 кадр
	float _speed;
	//сколько кадров будет жить пуля
	int _lifeTime;
	glm::vec2 _direction;
	glm::vec2 _position;
};

