#pragma once
#include <glm/glm.hpp>
#include <Bengine/SpriteBatch.h>

#include <Bengine/ResourceManager.h>
const float AGENT_WIDTH = 60.0f;
const float AGENT_RADIUS = (AGENT_WIDTH)/ 2.0f;
class Zombie;
class Human;


class Agent
{
public:
	Agent();
	//если есть виртуал, то вызывая дестр. агента, вызовутся дестр. наследуемых классов
	virtual ~Agent();
	//чистый виртуальный класс. мы больше не можем содавать объекты класса Агент, только его наследников.
	//по ссылке, ибо мы не хотим делать его копию. const ибо не хотим менять
	virtual void update(const std::vector<std::string> &levelData,
		std::vector <Human*> &humans,
		std::vector<Zombie*> &zombies, float deltaTime) = 0;

	bool collideWithLevel(const std::vector<std::string> &levelData);

	//будет считать коллайдить с одним и возвращать тру если было сопрокосновение
	bool collideWithAgent(Agent * agent);
	//ссылка или указатель. значение даст некорректную хуйню
	void draw(Bengine::SpriteBatch & _spriteBatch);
	//получение дамага, вернет true, если помер бедолага
	bool applyDamage(float damage);
	//получение позиции
	glm::vec2 getPosition()const { return _position; }

protected:
	//вектор спозициями и позиции угла
	void checkTilePosition(const std::vector<std::string> &levelData, std::vector<glm::vec2> & collideTilePositions,
		float x, float y);

	void collideWithTile(glm::vec2 tilePos);
	glm::vec2 _position;
	//всегда вправо
	glm::vec2 z_direction = glm::vec2(1.0f,0.0f);
	Bengine::Color _color;
	float _speed;
	float _health;
	GLuint z_textureID;

};

