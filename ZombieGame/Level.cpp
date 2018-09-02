#include "Level.h"
#include <fstream>
#include <iostream>
#include <Bengine/ResourceManager.h>
Level::Level(const std::string &fileName):
_numHumans(0)
{
	//����������, ����, ����, ����� ����� ���� �������� � ������������
	Bengine::Color whiteColor;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;
	std::ifstream file;
	file.open(fileName);

	//�������� �� ������
	if (file.fail()){
		Bengine::fatalError("�� ������� �������" +fileName);
	}
	//����� ������ ������ �� ��������� ����������
	std::string tmp;
	//�������(� ����������) ������� ������, ������� �������, ����� ��������� ���������� ������� ��� ������ �� �����
	file >> tmp >> _numHumans;
	std::getline(file, tmp);//���������� 1 ������
	while (std::getline(file, tmp)){
		_levelData.push_back(tmp);
	}
	_spriteBatch.init();
	//������������� ����� �� �������. ������ 1 ���, ��� ��� ��� ��� ���������?
	_spriteBatch.begin();

	//UV Rectangle
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	//������ ���� ������. ����� ���������� ��� auto
	for (int y = 0; y < _levelData.size(); y++){
		for (int x = 0; x < _levelData[y].size(); x++){
			//������ � ��������� �����
			char tile = _levelData[y][x];

			//��������� DestinationRectangle
			//x,y,width,height
			glm::vec4 destRect(x*TILE_WIDTH, y*TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);
			//��������� ����� �������
			switch (tile){
			case 'R':
				_spriteBatch.draw(destRect,
									uvRect,
									Bengine::ResourceManager::getTexture("Textures/dark_brick.png").id,
									0.0f,
									whiteColor);
				break;
			case 'G':
				_spriteBatch.draw(destRect,
									uvRect,
									Bengine::ResourceManager::getTexture("Textures/glass.png").id,
									0.0f,
									whiteColor);
				break;
			case 'L':
				_spriteBatch.draw(destRect,
									uvRect,
									Bengine::ResourceManager::getTexture("Textures/light_brick.png").id,
									0.0f,
									whiteColor);
				break;
			case'@':
				_levelData[y][x] = '.'; //c �����, ���� ���, ���� �� ���������.
				_startPlayerPos.x = x * TILE_WIDTH;
				_startPlayerPos.y = y * TILE_WIDTH;
				break;
			case 'Z':
				//emplace ��������� ���������� ������������
				_levelData[y][x] = '.'; //�� �� ��������� � ���������
				_zombieStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			case '.':
				break;
			default: 
				std::printf("������������ ������ %c at (%d,%d)\n", tile, x, y);
				//std::cout << "������������ ������" + tile + "� (" + x + " , " + y + ")";
			}
		}
	}

	_spriteBatch.end();
}


Level::~Level()
{
}

void Level::draw(){
	//������ ��� ��� ����� ���������� �������� ���
	_spriteBatch.renderBatch();

}