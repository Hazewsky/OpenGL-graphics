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
		//ortho -�������������
		//����� �������(�����), ������ �������(������ ������), ������ �������� ������� �������(������ ������)
		_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);

	}
	void Camera2D::update(){
		//���������� � ���� while
		if (_needsMatrixUpdate){
			//��� ����� ���� ��� ���������. ���������� �, � � z. ������������� ��������.
			//���� - ����� �� ������� ������ ������, ��� ��������� ���������� �����
			//����������� ����� ~_position.x+1
			glm::vec3 translate(-_position.x + _screenWidth/2, -_position.y +_screenHeight/2, 0.0f);
			//translate - ������� ������� 1- ��� �����������
			//2. ������, ������� ���������� ����������

			_cameraMatrix = glm::translate(_orthoMatrix, translate);
			//������ �������
			//��� ���������� �� ������� ����� ��������. ���� _scale==1, ������ �� ���������. <1 -����������, > 1 - ����������
			glm::vec3 scale(_scale, _scale, 0.0f);
			//������� scaling matrix(identity matrix)
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;
			//!!! ����� �������� � ����� ������� ��� ������. ������� - ��������, ����� �����
		
			_needsMatrixUpdate = false;
		}
	}

	glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords){
		//���� ������������� y, � �� ��� �������� ����
		screenCoords.y = _screenHeight - screenCoords.y;
		//������ 0,0 ����� ������ � width,height ������ ����� � ��� �����  -width/2, -height/2 ����� ������ � width/2, height/2 ������ �����
		//0,0 ����� ����������� � ������ ������. + ��� ������� ���� ��������
		screenCoords -= glm::vec2(_screenWidth/2, _screenHeight/2);

		//������ ������ ���������� �����. ��� ������� ���������� ������ ������ �� ��� (-300 == -100 ��� ����(3)) � �.�.
		screenCoords /= _scale;

		//translate � �������� ������. � ����������� �� ������� ������ ����� ���������� ��� � "����" �� �������
		//��� ������ �� 10, ���������� ����� +10 � �.�.
		screenCoords += _position;

		return screenCoords;
	}
	//������� AABB ����
	bool Camera2D::isBoxInView(const glm::vec2& position, const glm::vec2 dimensions){
		//��� � ������ ���������
		glm::vec2 scaledScreenDimensions = glm::vec2(_screenWidth, _screenHeight) / (_scale);
		//dimensions == �������, � ��� ���� ������. /2
		//_screenwidth/height(scalenScreenDim) == ���� ��������, � ����� �������
		const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDimensions.x/2.0f;
		const float MIN_DISTANCE_Y = dimensions.y /2.0f + scaledScreenDimensions.y/2.0f;
		//������ �� �������� �������� ����� �����
		//��� ������� ����������(�������?)
		glm::vec2 centerPos = position + dimensions/2.0f;
		//� ��� ����� ������
		glm::vec2 centerCameraPos = _position;
		//���� ����� ������� �� ������ ������ �� ������ ������
		glm::vec2 distVec = centerPos - centerCameraPos;
		//������� �� ������� ����� �� �������. ���.���������(��� ������ + ��� �����) - ����� �������
		//abs - ������. ��� ������ ����� �� ������� ���������, � ��� �� ������� ���� �� ��������. -(-distVec)
		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		//���� �����������, � ��� ���� ��������, ����� ������ ����� true, ��� �� ������ ������ ����� � �������� �������������
		if (xDepth > 0 && yDepth > 0){
			//���� ��������
			return true;
		}
		return false;
	}

}