#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace Bengine{
	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();
		void init(int screenWidth, int screenHeight);
		void update();

		//����� ���������� ������, ������������ � ������� � ����������
		//screen coords -> world coords -> return world coords
		glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);
		//����� camera cooling.
		//����. �������������� ������ �� ������ ��� ��������� �� ������� ����, ����������.�������� ����� �������.
		//��������� �� ��������������
		bool isBoxInView(const glm::vec2& position, const glm::vec2 dimensions);
		//����������
		void setPosition(const glm::vec2& newPosition){
			_position = newPosition;
			_needsMatrixUpdate = true;
		}
		void setScale(float newScale){
			_scale = newScale;
			_needsMatrixUpdate = true;
		}
		//���������
		glm::vec2 getPosition(){
			return _position;
		}
		float getScale(){
			return _scale;
		}
		glm::mat4 getCameraMatrix(){
			return _cameraMatrix;
		}
	private:
		int _screenWidth, _screenHeight;
		bool _needsMatrixUpdate;
		float _scale;
		//2 floats
		glm::vec2 _position;
		//4x4 matrix for orthographic + ���-�� ��� matrix
		glm::mat4 _cameraMatrix;
		glm::mat4 _orthoMatrix;
	};

}