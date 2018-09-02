#include "Bullet.h"

#include <Bengine/ResourceManager.h>

Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime) :
_position(pos),
_direction(dir),
_speed(speed),
_lifeTime(lifeTime)
{
}


Bullet::~Bullet()
{
}



void Bullet::draw(Bengine::SpriteBatch &spriteBatch){
	Bengine::Color color;
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	//вот эту херню мен€ть, так не надо делать, въебашить в инит текстуру котора€ требуетс€
	static Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("Textures/1.png");
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	//позиции и размер
	glm::vec4 posAndSize = glm::vec4(_position.x, _position.y, 30, 30);
	spriteBatch.draw(posAndSize, uv, texture.id, 0.0f, color);

}

bool Bullet::update(){
	//_direction должен быть unit. 1 там, 2. “ут еще и гравитацию можно заху€рить, ну тип - _gravity.Ќу это в платформере
	_position += _direction * _speed;
	_lifeTime--;
	if (_lifeTime == 0){
		return true;
	}
	return false;
}