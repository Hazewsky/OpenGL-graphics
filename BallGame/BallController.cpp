#include "BallController.h"



//обновляет шары
void BallController::updateBalls(std::vector <Ball> &balls, float deltaTime, int maxX, int maxY){
	//Трение
	const float FRICTION = 0.001f;
	//Оббновляем скорость
	if (grabbedBall_ != -1){
		balls[grabbedBall_].velocity = balls[grabbedBall_].position - prevPos_;
	}

	glm::vec2 gravity = getGravityAccel();

	for (size_t i = 0; i < balls.size(); i++){
		//приведения для умеьншения кода
		Ball &ball = balls[i];
		//обновляем движение если не взят шар
		if (i != grabbedBall_){
			ball.position += ball.velocity * deltaTime;
			//Вводим трение
			glm::vec2 momentumVec = ball.velocity * ball.mass;
			if (momentumVec.x != 0 || momentumVec.y != 0){
				if (FRICTION < glm::length(momentumVec)){
					ball.velocity -= deltaTime * FRICTION * glm::normalize(momentumVec) / ball.mass;
				}
				else{
					ball.velocity = glm::vec2(0.0f);
				}
			}
			//вводим гравитацию
			ball.velocity += gravity * deltaTime;
		}
		//ПРоверяем столкновение со стенами
		if (ball.position.x < 0){
			ball.position.x = 0;
			if (ball.velocity.x < 0) ball.velocity.x *= -1;
		}
		else if (ball.position.x + ball.radius * 2.0f >= maxX){
			ball.position.x + ball.radius * 2.0f - 1;
			if (ball.velocity.x > 0) ball.velocity *= -1;
		}
		if (ball.position.y < 0){
			ball.position.y = 0;
			if (ball.velocity.y < 0) ball.velocity.y *= -1;
		}
		else if (ball.position.y + ball.radius * 2.0f >= maxY){
			ball.position.y = maxY - ball.radius * 2.0f - 1;
			if (ball.velocity.y > 0)ball.velocity.y *= -1;
		}

		//проверка столкновений
		for (auto& it : balls){
			checkCollision(ball, it);
		}
	}
	//обновление взятого шара
	if (grabbedBall_ != -1){
		//Обновляем скорость снова, в случае если она изменится после столкновения
		balls[grabbedBall_].velocity = balls[grabbedBall_].position - prevPos_;
		prevPos_ = balls[grabbedBall_].position;
	}
}

//простые функции нажатия
void BallController::onMouseDown(std::vector<Ball> &balls, float mouseX, float mouseY){
	for (size_t i = 0; i < balls.size(); i++){
//проверяем, наведена ли мышь на шар
		if (isMouseOnBall(balls[i], mouseX, mouseY)){
//Внимание! Если меняется порядок шаров в векторе, это становится некорректным
			grabbedBall_ = i;
//Смещение курсора
			grabOffset_ = glm::vec2(mouseX, mouseY) - balls[i].position;
			prevPos_ = balls[i].position;
			balls[i].velocity = glm::vec2(0.0f);
		}
	}

}

void BallController::onMouseUp(std::vector<Ball> & balls){
	if (grabbedBall_ != -1){
		//Кидает мяч
		balls[grabbedBall_].velocity = balls[grabbedBall_].position - prevPos_;
		//сброс параметра
		grabbedBall_ = -1;
	}
}
void BallController::onMouseMove(std::vector<Ball> &balls, float mouseX, float mouseY){
	if (grabbedBall_ != -1){
		balls[grabbedBall_].position = glm::vec2(mouseX, mouseY) - grabOffset_;
	}

}
	//проверка столкновений между шарами
void BallController::checkCollision(Ball&b1, Ball&b2){
//Мы добавляем радиус когда позиция - это левый верхний угол
	glm::vec2 distVec = (b2.position + b2.radius) -
						(b1.position + b1.radius);
	glm::vec2 distDir = glm::normalize(distVec);
	float dist = glm::length(distVec);
	float totalRadius = b1.radius + b2.radius;

	float collisionDepth = totalRadius - dist;
//Проверка на коллизию
	if (collisionDepth > 0){
		//отталкивает меньший
		if (b1.mass < b2.mass){
			b1.position -= distDir * collisionDepth;
		}else{
			b2.position += distDir * collisionDepth;
		}
		//Вычисление отклонения http://stackoverflow.com/a/345863
		//умноежение векторов dot
		float aci = glm::dot(b1.velocity, distDir) / b2.mass;
		float bci = glm::dot(b2.velocity, distDir) / b1.mass;
		//Соотношение масс
		float massRatio = b1.mass / b2.mass;

		b1.velocity += (bci - aci)*distDir*(1.0f / massRatio);
		b2.velocity += (aci - bci) * distDir *massRatio;
	}
}
	//Возвращает истину если мышь наведена на шар
bool BallController::isMouseOnBall(Ball&b, float mouseX, float mouseY){
	return(mouseX >= b.position.x && mouseX < b.position.x + b.radius * 2.0f &&
		mouseY >= b.position.y && mouseY < b.position.y * b.radius * 2.0f);

}

glm::vec2 BallController::getGravityAccel(){
	const float GRAVITY_FORCE = 0.1f;
	glm::vec2 gravity;

	switch (gravityDirection_){
	case GravityDirection::DOWN:
		gravity = glm::vec2(0.0f, -GRAVITY_FORCE);
		break;
	case GravityDirection::LEFT:
		gravity = glm::vec2(-GRAVITY_FORCE, 0.0f);
		break;
	case GravityDirection::RIGHT:
		gravity = glm::vec2(GRAVITY_FORCE, 0.0f);
		break;
	case GravityDirection::UP:
		gravity = glm::vec2(0.0f, GRAVITY_FORCE);
		break;
	default:
		gravity = glm::vec2(0.0f);
	}
	return gravity;
}