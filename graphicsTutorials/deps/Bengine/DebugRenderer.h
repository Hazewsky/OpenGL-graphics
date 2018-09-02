#pragma once

#include "GLSLProgram.h"
#include "Vertex.h"
#include <glm/glm.hpp>
#include <vector>
namespace Bengine{
class DebugRenderer
	//�������� �������� ������ ����� � ���� �����
{
public:
	DebugRenderer();
	~DebugRenderer();
	//��������� ��������
	void init();

	void end();

	void drawBox(const glm::vec4& destRect, const Color color, float angle);

	void drawCircle(const glm::vec2& center, const Color color, float radius);

	void render();
	//opposite of init 
	void dispose();

	struct DebugVertex{
		glm::vec2 position;
		Bengine::Color color;
	};
private:
	Bengine::GLSLProgram m_program;
	//indices?
	std::vector<DebugVertex> m_verts;
	//draw_arrays ������ ��������������� �� ������, ������ �� � ��������� ����� ������� � ������ ������ ����� �������� �������
	//����� ���� ��������, �� ���������� ����� ��� ���� ��������
	//� ������ ���-�� ����� indecis = {}0,1 - ������ �����, 1,2 - �������, 2,3 - ������, 3,0-���������
	std::vector<GLuint> m_indices;
	GLuint m_vbo, m_vao, m_ibo;
		//vertex buffer object, vertex array object, index buffer object

};

}

