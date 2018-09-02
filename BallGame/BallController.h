#pragma once

#include <vector>
#include "Ball.h"
enum class GravityDirection{NONE, LEFT,UP,RIGHT,DOWN};

//Ћогика шаров
class BallController
{
public:
	//обновл€ет шары
	void updateBalls(std::vector <Ball> &balls, float deltaTime, int maxX, int maxY);
	//простые функции нажати€
	void onMouseDown(std::vector<Ball> &balls, float mouseX, float mouseY);
	void onMouseUp(std::vector<Ball> & balls);
	void onMouseMove(std::vector<Ball> &balls, float mouseX, float mouseY);
	void setGravityDirecton(GravityDirection dir){ gravityDirection_ = dir; }
private:
	//проверка столкновений между шарами
	void checkCollision(Ball&b1, Ball&b2);
	//¬озвращает истину если мышь наведена на шар
	bool isMouseOnBall(Ball&b, float mouseX, float mouseY);
	glm::vec2 getGravityAccel();

	int grabbedBall_ = -1; ///<Ўар который мы в данный момент схватили
	glm::vec2 prevPos_ = glm::vec2(0.0f); ///<предыдуща€ позици€ вз€того шара
	glm::vec2 grabOffset_ = glm::vec2(0.0f); ///< смещение курсора на выбранный шар

	//гравитаци€ по умолчанию
	GravityDirection gravityDirection_ = GravityDirection::NONE;
};

