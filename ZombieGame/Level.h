#pragma once
#include <string>
#include <vector>
#include <Bengine/BengineErrors.h>
#include <Bengine/SpriteBatch.h>

const int TILE_WIDTH = 64;

class Level
{
public:
	//Load the level
	Level(const std::string &fileName );
	~Level();
	//����� � ����� ����

	void draw();
	
	//get
	//x
	int getWidth()const { return _levelData[0].size(); }
	//y
	int getHeight()const { return _levelData.size(); }
	const std::vector<std::string> & getLevelData()const { return _levelData; }
	int getNumHumans() const { return _numHumans; }
	glm::vec2 getStartPlayerPos() const {	return _startPlayerPos;}
	//���� �� ����� ��������
	const std::vector<glm::vec2>& getZombieStartPos() const { return _zombieStartPositions; }

private:
	std::vector<std::string> _levelData;
	int _numHumans;
	Bengine::SpriteBatch _spriteBatch;
	//Dependensies ejection. �� ������� ������ ����� ������ ��������. ��� ��������� � ��������� � mainGame
	//�� �.�. ��� ����������� �����, ��� �� ������ �� �����. � ��� �� ����� ���� ���� 1
	//Bengine::ResourceManager * _resourceManager;
	//��������� ������� ������. ivec2 2 ��������� ��������� map<int,int>
	glm::vec2 _startPlayerPos;
	// ������ ��������, ��� �������� ��������� ������������ �����
	std::vector<glm::vec2> _zombieStartPositions;
};

