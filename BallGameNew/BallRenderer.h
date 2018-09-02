#pragma once

#include <Bengine/SpriteBatch.h>
#include <Bengine/GLSLProgram.h>
#include <vector>
#include <memory>
#include "Ball.h"

class BallRenderer {
public:
	
	////lazy init. будет инициировать ГЛСЛ только когда оно использунется в функции
    virtual void renderBalls(Bengine::SpriteBatch& spriteBatch,
							const std::vector<Ball>&balls,
							const glm::mat4& projectionMatrix);

protected:
	std::unique_ptr<Bengine::GLSLProgram> m_program = nullptr;
};


class MomentumBallRenderer :public BallRenderer{
public:
	virtual void renderBalls(Bengine::SpriteBatch& spriteBatch,
		const std::vector<Ball>&balls,
		const glm::mat4& projectionMatrix) override;
};

class VelocityBallRenderer :public BallRenderer{
public:
	VelocityBallRenderer(int screenWidth, int screenHeight);
	virtual void renderBalls(Bengine::SpriteBatch& spriteBatch,
		const std::vector<Ball>&balls,
		const glm::mat4& projectionMatrix) override;

private:
	int m_screenWidth, m_screenHeight;
};

class WeirdRenderer :public BallRenderer{
public:
	WeirdRenderer(int screenWidth, int screenHeight);
	virtual void renderBalls(Bengine::SpriteBatch& spriteBatch,
		const std::vector<Ball>&balls,
		const glm::mat4& projectionMatrix) override;

private:
	int m_screenWidth, m_screenHeight;
};