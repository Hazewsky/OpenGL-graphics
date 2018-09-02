#pragma once

#include <glm/glm.hpp>
#include <Bengine/Vertex.h>
//SRP - Single Responsibility principle
//У каждого класса должно быть свое поле ответственности
//POD
//данные Ball
struct Ball{

	Ball(float Radius, float Mass, const glm::vec2& Pos,
		const glm::vec2& Vel, unsigned int TextureId,
		const Bengine::Color& Color);

	float radius;
	float mass;
	glm::vec2 velocity;
	glm::vec2 position;
	unsigned int textureId = 0;
	Bengine::Color color;
};

