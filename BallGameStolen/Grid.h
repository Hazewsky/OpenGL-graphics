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
	//��������� ��� � ���������� � ����� ������ �� �����������
	void addBall(Ball* ball);
	//��������� ��� � ������������ ������
	void addBall(Ball* ball,Cell* cell);
	//�������� ������ �� ��������� ��������� ������
	Cell* getCell(int x, int y);
	//� ��� ��� �������� ������ �� ��������� ��������� ����(����������), �� ��������� ������� ���� �����
	Cell* getCell(const glm::vec2 pos);

	void RemoveBallFromCell(Ball* ball);

private:
	//����� �� ����� 2� ������ �� ������� ������ ������� �� ������ ��� � 1�.
	//� ���� cache friendl. �� ������� 1� ������, �� ����� ������� ��� 2�
	std::vector<Cell> m_cells;
	int m_cellSize, m_width, m_height, m_numXCells = 0, m_numYCells = 0;
};

