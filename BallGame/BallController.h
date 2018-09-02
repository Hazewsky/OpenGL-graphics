#pragma once

#include <vector>
#include "Ball.h"
enum class GravityDirection{NONE, LEFT,UP,RIGHT,DOWN};

//������ �����
class BallController
{
public:
	//��������� ����
	void updateBalls(std::vector <Ball> &balls, float deltaTime, int maxX, int maxY);
	//������� ������� �������
	void onMouseDown(std::vector<Ball> &balls, float mouseX, float mouseY);
	void onMouseUp(std::vector<Ball> & balls);
	void onMouseMove(std::vector<Ball> &balls, float mouseX, float mouseY);
	void setGravityDirecton(GravityDirection dir){ gravityDirection_ = dir; }
private:
	//�������� ������������ ����� ������
	void checkCollision(Ball&b1, Ball&b2);
	//���������� ������ ���� ���� �������� �� ���
	bool isMouseOnBall(Ball&b, float mouseX, float mouseY);
	glm::vec2 getGravityAccel();

	int grabbedBall_ = -1; ///<��� ������� �� � ������ ������ ��������
	glm::vec2 prevPos_ = glm::vec2(0.0f); ///<���������� ������� ������� ����
	glm::vec2 grabOffset_ = glm::vec2(0.0f); ///< �������� ������� �� ��������� ���

	//���������� �� ���������
	GravityDirection gravityDirection_ = GravityDirection::NONE;
};

