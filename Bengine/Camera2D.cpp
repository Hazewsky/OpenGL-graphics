#include "Camera2D.h"
#include <iostream>
namespace Bengine{
	//initialization list
	Camera2D::Camera2D() :
		_position(0.0f, 0.0f),
		_cameraMatrix(1.0f),
		_orthoMatrix(1.0f),
		_scale(1),
		_needsMatrixUpdate(1),
		_screenWidth(500),
		_screenHeight(500)

	{

	}


	Camera2D::~Camera2D()
	{
	}
	void Camera2D::init(int screenWidth, int screenHeight){
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		//ortho -инициализация
		//левая граница(флоат), правая граница(ширина экрана), нижняя границаб верхняя граница(высота экрана)
		_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);

	}
	void Camera2D::update(){
		//переделать в цикл while
		if (_needsMatrixUpdate){
			//тут могут быть три параметра. Трансляция х, у и z. Отрицательные значения.
			//Суть - когда мы двигаем объект вправо, все остальное сдвигается влево
			//Попробовать через ~_position.x+1
			glm::vec3 translate(-_position.x + _screenWidth/2, -_position.y +_screenHeight/2, 0.0f);
			//translate - двигаем объектю 1- что транслируем
			//2. вектор, который закодирует трансляцию

			_cameraMatrix = glm::translate(_orthoMatrix, translate);
			//теперь скейлим
			//Тут показывает на сколько будет скейлить. Если _scale==1, ничего не изменится. <1 -уменьшится, > 1 - увеличится
			glm::vec3 scale(_scale, _scale, 0.0f);
			//создаем scaling matrix(identity matrix)
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;
			//!!! ИМЕЕТ ЗНАЧЕНИЕ В КАКОМ ПОРЯДКЕ ЭТО ДЕЛАТЬ. СНАЧАЛА - ТРАНЛЕЙТ, ПОТОМ СКЕЙЛ
		
			_needsMatrixUpdate = false;
		}
	}

	glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords){
		//надо инвертировать y, а то оно наоборот идет
		screenCoords.y = _screenHeight - screenCoords.y;
		//вместо 0,0 слева сверху и width,height справа снизу у нас будет  -width/2, -height/2 слева сверху и width/2, height/2 справа снизу
		//0,0 будет прямохонько в центре экрана. + это удобнее дабы скейлить
		screenCoords -= glm::vec2(_screenWidth/2, _screenHeight/2);

		//теперь делаем корректный скейл. При зуминге координаты должно делить на зум (-300 == -100 при зуме(3)) и т.д.
		screenCoords /= _scale;

		//translate с позицией камеры. В зависимости от позиции камеры будет показывать где в "мире" мы кликаем
		//При сдвиге на 10, координаты будут +10 и т.д.
		screenCoords += _position;

		return screenCoords;
	}
	//простой AABB тест
	bool Camera2D::isBoxInView(const glm::vec2& position, const glm::vec2 dimensions){
		//это с учетом скейлинга
		glm::vec2 scaledScreenDimensions = glm::vec2(_screenWidth, _screenHeight) / (_scale);
		//dimensions == диаметр, а нам надо радиус. /2
		//_screenwidth/height(scalenScreenDim) == тоже диаметры, а нужны радиусы
		const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDimensions.x/2.0f;
		const float MIN_DISTANCE_Y = dimensions.y /2.0f + scaledScreenDimensions.y/2.0f;
		//теперь мы проводим рассчеты через центр
		//это позиция параметров(агентов?)
		glm::vec2 centerPos = position + dimensions/2.0f;
		//а это центр камеры
		glm::vec2 centerCameraPos = _position;
		//ищем длину вектора от центра агента до центра камеры
		glm::vec2 distVec = centerPos - centerCameraPos;
		//смотрим на сколько зашло за пределы. Мин.дистанция(рад агента + рад тайла) - длина вектора
		//abs - модуль. нам просто срать на порядок вычитания, и так мы избавим себя от сложения. -(-distVec)
		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		//если выполняется, у нас есть коллизия, почти всегда будет true, ибо мы храним только тайлы с которыми соприкасаемся
		if (xDepth > 0 && yDepth > 0){
			//Была коллизия
			return true;
		}
		return false;
	}

}