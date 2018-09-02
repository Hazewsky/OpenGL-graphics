#include "Ball.h"

Ball::Ball(float Radius, float Mass, const glm::vec2& Pos,
	const glm::vec2& Vel, unsigned int TextureId,
	const Bengine::Color& Color){
	this->radius = Radius;
	this->mass = Mass;
	this->position = Pos;
	this->velocity = Vel;
	this->textureId = TextureId;
	this->color = Color;
}