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
	//no access violation ���������� ����� ��� random access violation
	Cell* ownerCell = nullptr;
	//��������� ���� ������� ������� �� ��������� ���� ������(� ������ ������) ��� ����������� ����
	int cellVectorIndex = -1;
};
