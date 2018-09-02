#pragma once

#include <glm/glm.hpp>
#include <Bengine/Vertex.h>


struct Cell;
// POD
struct Ball {
    Ball(float radius, float mass, const glm::vec2& pos,
         const glm::vec2& vel, unsigned int textureId,
         const Bengine::Color& color);

    float radius; 
    float mass; 
    glm::vec2 velocity;
    glm::vec2 position;
    unsigned int textureId = 0;
    Bengine::Color color;
	//no access violation отдебажить легче чем random access violation
	Cell* ownerCell = nullptr;
	//небольшой трюк который поможет не проходить весь вектор(в худшем случае) для перемещения шара
	int cellVectorIndex = -1;
};
