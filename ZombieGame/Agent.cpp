#include "Agent.h"
#include <Bengine/ResourceManager.h>
#include "Level.h"
#include <algorithm>

Agent::Agent()
{
}


Agent::~Agent()
{
}


bool Agent::collideWithLevel(const std::vector<std::string> &levelData){
	//если игрок больше, то  и сравнивать надо с большим кол-вом тайлов
	//все наши тайлы на одной и той же позиции, потому можем не создавать структуру где есть тайл и его коорд.
	//здесь будут позиции
	std::vector<glm::vec2> collideTilePositions;
	//чекаем четыре угла
	//1
	checkTilePosition(levelData,
		collideTilePositions,
		_position.x,
		_position.y);
	//2
	checkTilePosition(levelData,
		collideTilePositions,
		_position.x+AGENT_WIDTH,
		_position.y);
	//3
	checkTilePosition(levelData,
		collideTilePositions,
		_position.x,
		_position.y+AGENT_WIDTH);
	//4
	checkTilePosition(levelData,
		collideTilePositions,
		_position.x+AGENT_WIDTH,
		_position.y+AGENT_WIDTH);

	if(collideTilePositions.size() == 0) {return false; }
	//если коллайдится с одним из solid блоков, то добавить его координаты в вектор
	//ебашь коллизию
	for (auto& it : collideTilePositions){
		collideWithTile(it);

	}
	return true;
}

//будет круговая коллизия
bool Agent::collideWithAgent(Agent * agent){
	//т.к. размеры одинаковые, можно вычислять с помощью углов, но все равно будет черзе центр. ИБО вдруг они будут разного размера
	//универсальность, бля
	const float MIN_DISTANCE = 2.0f * AGENT_RADIUS;

	glm::vec2 centerPosA = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	//длина вектора между двумя центрами
	float distance = glm::length(distVec);
	//глубина коллизии
	float collisionDepth = MIN_DISTANCE - distance;
	if (collisionDepth > 0){
		//забавная херь, когда гг поглощается мобом. можно управлять им, но и сам он может управлять тобой
		/*_position -= distVec / 2.0f;
		agent->_position += distVec / 2.0f;*/
		glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;
		_position += collisionDepthVec/5.0f;
		agent->_position -= collisionDepthVec/5.0f;
		return true;
	}
	return false;
}



void Agent::draw(Bengine::SpriteBatch & _spriteBatch){


	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = AGENT_WIDTH;
	destRect.w = AGENT_WIDTH;
	_spriteBatch.draw(destRect, uvRect, z_textureID, 0.0f, _color, z_direction);
}

void Agent::checkTilePosition(const std::vector<std::string> &levelData, 
							std::vector<glm::vec2> & collideTilePositions,
							float x, float y){
	//первый угол
	
	glm::vec2 cornerPos = glm::ivec2(floor(x) / (float)TILE_WIDTH,
		floor(y) / float(TILE_WIDTH));

	//чеким, чтобы не выбивало ошибку если агент спавнится вне мира. Коллизится и выебывает sunscript out of range
	//проверочка, чтобы ни одна тварь не спавнилась вне мира
	//слева за границей или справа
	//сверху или снизу
	if (cornerPos.x < 0 || cornerPos.x >= levelData[0].length() ||
		cornerPos.y < 0 || cornerPos.y >= levelData.size()){
		return;
	}
	//проверяем, можно ли коллайдить вообще с этим углом
	//умножаем на TILE_WIDTH что это реально плавающая точка
	if (levelData[cornerPos.y][cornerPos.x] != '.'){
		//+вектор из-за того что раньше хранились коорд. левого верхнего угла. теперь - центра. Так легче
		collideTilePositions.push_back(cornerPos * float(TILE_WIDTH) + glm::vec2((float)TILE_WIDTH/2.0f));
	}
}

bool Agent::applyDamage(float damage){
	_health -= damage;
	if (_health <= 0){
		return true;
	}
	return false;
}


//access align box collision AABB. Одно из примитивнейших
void Agent::collideWithTile(glm::vec2 tilePos){
	
	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	//минимальная дистанция, которая у нас может быть между агентов и тайлом
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;
	//теперь мы проводим рассчеты через центр
	glm::vec2 centerPlayerPos = _position + glm::vec2(AGENT_RADIUS);
	//ищем длину вектора от центра игрока до центра текстуры
	glm::vec2 distVec = centerPlayerPos - tilePos;
	//смотрим на сколько зашло за пределы. Мин.дистанция(рад агента + рад тайла) - длина вектора
	//abs - модуль. нам просто срать на порядок вычитания, и так мы избавим себя от сложения. -(-distVec)
	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	//если выполняется, у нас есть коллизия, почти всегда будет true, ибо мы храним только тайлы с которыми соприкасаемся
	if (xDepth > 0 && yDepth > 0){
		/*if (xDepth < MIN_DISTANCE)	_position.x -= xDepth;
		else if (yDepth < MIN_DISTANCE)_position.y -= yDepth;*/
		//смотрим что меньше. Мы вытакливаем в сторону меньшего значения
		//max вернет максимальное значение. должно быть больше нуля, поэтому 0.0f
		if (std::max(xDepth, 0.0f) < std::max(yDepth,0.0f)){
			//в зависимости от длины вектора между центрами
			if (distVec.x < 0)_position.x -= xDepth;
			else _position.x += xDepth;
		}
		else{
			if (distVec.y < 0)_position.y -= yDepth;
			else _position.y += yDepth;
		}
		
	}
	//можно попробовать через абсолютные значения и мой вариант кода
	//тут есть баг у вертикальных поверхностей, когда сначала коллайдишь с ними и пытаешься идти вниз, то застряешь
	//сортировать по дистанции и коллайдить ближайшие сначала
}