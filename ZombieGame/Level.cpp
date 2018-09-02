#include "Level.h"
#include <fstream>
#include <iostream>
#include <Bengine/ResourceManager.h>
Level::Level(const std::string &fileName):
_numHumans(0)
{
	//Переделать, сука, цвет, чтобы можно было задавать в конструкторе
	Bengine::Color whiteColor;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;
	std::ifstream file;
	file.open(fileName);

	//проверка на ошибки
	if (file.fail()){
		Bengine::fatalError("Не удалось открыть" +fileName);
	}
	//пилим первую строку во временную переменную
	std::string tmp;
	//вбиваем(и выкидываем) сначала строку, которую игнорим, потом запиываем количество человек для спавна из файла
	file >> tmp >> _numHumans;
	std::getline(file, tmp);//выкидываем 1 строку
	while (std::getline(file, tmp)){
		_levelData.push_back(tmp);
	}
	_spriteBatch.init();
	//инициализация батча на локацию. Делаем 1 раз, ибо нах нам его обновлять?
	_spriteBatch.begin();

	//UV Rectangle
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	//рендер всех клеток. Потом переделать под auto
	for (int y = 0; y < _levelData.size(); y++){
		for (int x = 0; x < _levelData[y].size(); x++){
			//символ в текстовом файле
			char tile = _levelData[y][x];

			//получение DestinationRectangle
			//x,y,width,height
			glm::vec4 destRect(x*TILE_WIDTH, y*TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);
			//обработка этого символа
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
				_levelData[y][x] = '.'; //c собой, ясен хер, тоже не коллайдим.
				_startPlayerPos.x = x * TILE_WIDTH;
				_startPlayerPos.y = y * TILE_WIDTH;
				break;
			case 'Z':
				//emplace позволяет вхуяривать конструкторы
				_levelData[y][x] = '.'; //мы не коллайдим с зомбаками
				_zombieStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			case '.':
				break;
			default: 
				std::printf("Некорректный символ %c at (%d,%d)\n", tile, x, y);
				//std::cout << "Некорректный символ" + tile + "в (" + x + " , " + y + ")";
			}
		}
	}

	_spriteBatch.end();
}


Level::~Level()
{
}

void Level::draw(){
	//каждый раз как хотим отрисовать вызываем это
	_spriteBatch.renderBatch();

}