#include "Grid.h"


Grid::Grid(int width, int height, int cellSize) :
m_width(width),
m_height(height),
m_cellSize(cellSize)
{
	//ceill ��������� � ������� �������. ��� ����� ������� �� ������� ���� ������. ��� ����� �����
	m_numXCells = ceill((float)m_width / m_cellSize);
	m_numYCells = ceill((float)m_height / m_cellSize);
	//�� ��������� ������ ��� ���������� ������
	//int Array[3][3] == Array[9];
	//������� * �������
	const int BALLS_TO_RESERVE = 20;
	m_cells.resize(m_numYCells * m_numXCells);
	for (int i = 0; i < m_cells.size(); i++){
		m_cells[i].balls.reserve(BALLS_TO_RESERVE);
	}
}


Grid::~Grid()
{
}

void Grid::addBall( Ball* ball){
	//��������� � ����� ������ ��� � ��������� � ��� ���
	Cell* cell = getCell(ball->position);
	//� ��������� balls
	cell->balls.push_back(ball);
	//����������� ��� ������
	//&cell ����� ���� ����� � local reference. ��� �� ������.
	//points to referencies ����� ����
	ball->ownerCell = cell;
	//������� � �������(���� � ��� �������)
	ball->cellVectorIndex = cell->balls.size() - 1;

	//��� ����� �������, �� ��� ������. ����� �� ���� ����� �������...function call overhead
	//addBall(ball, cell);
}


void Grid::addBall(Ball* ball, Cell* cell){
	//� ��������� balls
	cell->balls.push_back(ball);
	//����������� ��� ������
	//&cell ����� ���� ����� � local reference. ��� �� ������.
	//points to referencies ����� ����
	ball->ownerCell = cell;
	//������� � �������(���� � ��� �������)
	ball->cellVectorIndex = cell->balls.size() - 1;
}
Cell* Grid::getCell(int x, int y){
	if (x < 0) x = 0;
	if (x >= m_numXCells) x = m_numXCells -1;
	if (y < 0) y = 0;
	if (y >= m_numYCells) y = m_numYCells -1;
	//������� �������������� + ����������. ��� �������� ������� 1� ������ ��� 2�
	//���������� ���������
	return &m_cells[y * m_numXCells + x];
}

Cell* Grid::getCell(const glm::vec2 pos){
	/*if (pos.x < 0) return m_cells[0];///<���������� ������ ������ �� �������
	if (pos.x >= m_width) return m_cells[0];//return m_cells[m_numXCells - 1];
	if (pos.y < 0) return m_cells[0];
	if (pos.y >= m_height) return  m_cells[0];*/
	
	//�� ����� �������� �� ������ �� �� ������������� �������, ���, ��� ��������

	int cellX = (int)(pos.x / m_cellSize);
	int cellY = (int)(pos.y / m_cellSize);

	return getCell(cellX, cellY);
}


void Grid::RemoveBallFromCell(Ball* ball){
	std::vector<Ball*>& balls = ball->ownerCell->balls;
	//NOrmal vector swap
	balls[ball->cellVectorIndex] = balls.back();
	balls.pop_back();
	//Update vector index
	//���� ������ �� ��������� �����, ������ ������, ������ ����� �������
	if (ball->cellVectorIndex < balls.size()){
		balls[ball->cellVectorIndex]->cellVectorIndex = ball->cellVectorIndex;
	}
	//set the index of ball to -1
	ball->cellVectorIndex = -1;
	ball->ownerCell = nullptr;
}
