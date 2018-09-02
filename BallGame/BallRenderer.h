#pragma once

#include <Bengine/SpriteBatch.h>
#include "Ball.h"
//Decouling the data
//Суть в том что на каждый новый метод отрисовки - новый класс
//отрисовавывает шары
class BallRenderer
{
public:
	void renderBall(Bengine::SpriteBatch& spriteBatch, Ball& ball);
};

