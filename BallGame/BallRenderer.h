#pragma once

#include <Bengine/SpriteBatch.h>
#include "Ball.h"
//Decouling the data
//���� � ��� ��� �� ������ ����� ����� ��������� - ����� �����
//�������������� ����
class BallRenderer
{
public:
	void renderBall(Bengine::SpriteBatch& spriteBatch, Ball& ball);
};

