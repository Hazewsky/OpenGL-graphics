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
	//���� ����� ������, ��  � ���������� ���� � ������� ���-��� ������
	//��� ���� ����� �� ����� � ��� �� �������, ������ ����� �� ��������� ��������� ��� ���� ���� � ��� �����.
	//����� ����� �������
	std::vector<glm::vec2> collideTilePositions;
	//������ ������ ����
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
	//���� ����������� � ����� �� solid ������, �� �������� ��� ���������� � ������
	//����� ��������
	for (auto& it : collideTilePositions){
		collideWithTile(it);

	}
	return true;
}

//����� �������� ��������
bool Agent::collideWithAgent(Agent * agent){
	//�.�. ������� ����������, ����� ��������� � ������� �����, �� ��� ����� ����� ����� �����. ��� ����� ��� ����� ������� �������
	//���������������, ���
	const float MIN_DISTANCE = 2.0f * AGENT_RADIUS;

	glm::vec2 centerPosA = _position + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	//����� ������� ����� ����� ��������
	float distance = glm::length(distVec);
	//������� ��������
	float collisionDepth = MIN_DISTANCE - distance;
	if (collisionDepth > 0){
		//�������� ����, ����� �� ����������� �����. ����� ��������� ��, �� � ��� �� ����� ��������� �����
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
	//������ ����
	
	glm::vec2 cornerPos = glm::ivec2(floor(x) / (float)TILE_WIDTH,
		floor(y) / float(TILE_WIDTH));

	//�����, ����� �� �������� ������ ���� ����� ��������� ��� ����. ���������� � ��������� sunscript out of range
	//����������, ����� �� ���� ����� �� ���������� ��� ����
	//����� �� �������� ��� ������
	//������ ��� �����
	if (cornerPos.x < 0 || cornerPos.x >= levelData[0].length() ||
		cornerPos.y < 0 || cornerPos.y >= levelData.size()){
		return;
	}
	//���������, ����� �� ���������� ������ � ���� �����
	//�������� �� TILE_WIDTH ��� ��� ������� ��������� �����
	if (levelData[cornerPos.y][cornerPos.x] != '.'){
		//+������ ��-�� ���� ��� ������ ��������� �����. ������ �������� ����. ������ - ������. ��� �����
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


//access align box collision AABB. ���� �� ��������������
void Agent::collideWithTile(glm::vec2 tilePos){
	
	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	//����������� ���������, ������� � ��� ����� ���� ����� ������� � ������
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;
	//������ �� �������� �������� ����� �����
	glm::vec2 centerPlayerPos = _position + glm::vec2(AGENT_RADIUS);
	//���� ����� ������� �� ������ ������ �� ������ ��������
	glm::vec2 distVec = centerPlayerPos - tilePos;
	//������� �� ������� ����� �� �������. ���.���������(��� ������ + ��� �����) - ����� �������
	//abs - ������. ��� ������ ����� �� ������� ���������, � ��� �� ������� ���� �� ��������. -(-distVec)
	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	//���� �����������, � ��� ���� ��������, ����� ������ ����� true, ��� �� ������ ������ ����� � �������� �������������
	if (xDepth > 0 && yDepth > 0){
		/*if (xDepth < MIN_DISTANCE)	_position.x -= xDepth;
		else if (yDepth < MIN_DISTANCE)_position.y -= yDepth;*/
		//������� ��� ������. �� ����������� � ������� �������� ��������
		//max ������ ������������ ��������. ������ ���� ������ ����, ������� 0.0f
		if (std::max(xDepth, 0.0f) < std::max(yDepth,0.0f)){
			//� ����������� �� ����� ������� ����� ��������
			if (distVec.x < 0)_position.x -= xDepth;
			else _position.x += xDepth;
		}
		else{
			if (distVec.y < 0)_position.y -= yDepth;
			else _position.y += yDepth;
		}
		
	}
	//����� ����������� ����� ���������� �������� � ��� ������� ����
	//��� ���� ��� � ������������ ������������, ����� ������� ���������� � ���� � ��������� ���� ����, �� ���������
	//����������� �� ��������� � ���������� ��������� �������
}