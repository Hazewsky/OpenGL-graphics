#include "Grid.h"


Grid::Grid(int width, int height, int cellSize) :
m_width(width),
m_height(height),
m_cellSize(cellSize)
{
	//ceill округл€ет в большую сторону. Ёто чтобы деление не сожрало одну €чейку. “ак будет запас
	m_numXCells = ceill((float)m_width / m_cellSize);
	m_numYCells = ceill((float)m_height / m_cellSize);
	//на удивление хранит как одномерный массив
	//int Array[3][3] == Array[9];
	//колонки * строчки
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
	//провер€ем в какой €чейке шар и добавл€ем в нее шар
	Cell* cell = getCell(ball->position);
	//в структуре balls
	cell->balls.push_back(ball);
	//присваиваем шар €чейке
	//&cell может дать адрес к local reference. “ак не делать.
	//points to referencies елать низ€
	ball->ownerCell = cell;
	//позици€ в векторе(если в зад впилили)
	ball->cellVectorIndex = cell->balls.size() - 1;

	//так можно сделать, но это опасно. лучше не надо может вызвать...function call overhead
	//addBall(ball, cell);
}


void Grid::addBall(Ball* ball, Cell* cell){
	//в структуре balls
	cell->balls.push_back(ball);
	//присваиваем шар €чейке
	//&cell может дать адрес к local reference. “ак не делать.
	//points to referencies елать низ€
	ball->ownerCell = cell;
	//позици€ в векторе(если в зад впилили)
	ball->cellVectorIndex = cell->balls.size() - 1;
}
Cell* Grid::getCell(int x, int y){
	if (x < 0) x = 0;
	if (x >= m_numXCells) x = m_numXCells -1;
	if (y < 0) y = 0;
	if (y >= m_numYCells) y = m_numYCells -1;
	//площадь пр€моугольника + координата. Ёто позволит считать 1д массив как 2д
	//возвращаем указатель
	return &m_cells[y * m_numXCells + x];
}

Cell* Grid::getCell(const glm::vec2 pos){
	/*if (pos.x < 0) return m_cells[0];///<возвращает первую клетку по дефолту
	if (pos.x >= m_width) return m_cells[0];//return m_cells[m_numXCells - 1];
	if (pos.y < 0) return m_cells[0];
	if (pos.y >= m_height) return  m_cells[0];*/
	
	//мы юзаем проверку на ошибки из не перегруженной функции, лол, это легально

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
	//если больше не останетс€ шаров, выбьет ошибку, потому такое условие
	if (ball->cellVectorIndex < balls.size()){
		balls[ball->cellVectorIndex]->cellVectorIndex = ball->cellVectorIndex;
	}
	//set the index of ball to -1
	ball->cellVectorIndex = -1;
	ball->ownerCell = nullptr;
}
