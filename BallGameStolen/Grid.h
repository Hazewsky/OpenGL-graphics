#pragma once
#include "Ball.h"
#include <vector>

struct Cell{
	//8 bytes
	std::vector<Ball*> balls;

};
class Grid
{
	friend class BallController;
public:
	Grid(int width, int height, int cellSize);
	~Grid();
	//ƒобавл€ет шар и определ€ет к какой €чейке он принадлежит
	void addBall(Ball* ball);
	//ƒќбавл€ет шар к определенной €чейке
	void addBall(Ball* ball,Cell* cell);
	//получаем клетку на основании коррдинат клетки
	Cell* getCell(int x, int y);
	//а тут уже получаем клетку на оснвоании координат окна(актуальных), на основании позиции шара короч
	Cell* getCell(const glm::vec2 pos);

	void RemoveBallFromCell(Ball* ball);

private:
	//когда мы юзаем 2д вектор мы гораздо больше прыгаем по пам€ти чем с 1д.
	//и ради cache friendl. мы возьмем 1д вектор, но будем считать его 2д
	std::vector<Cell> m_cells;
	int m_cellSize, m_width, m_height, m_numXCells = 0, m_numYCells = 0;
};

