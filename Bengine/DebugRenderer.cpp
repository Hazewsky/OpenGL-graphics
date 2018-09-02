#include "DebugRenderer.h"

namespace Bengine{
DebugRenderer::DebugRenderer()
{
}


DebugRenderer::~DebugRenderer()
{
	dispose();
}

void DebugRenderer::init(){
	//����. ��������

	//������������� �������
	//����������, ������
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	//������
	glBindVertexArray(m_vao);
	//���� ������
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	//������ ���� ��������� ������� �����.
	//��� �� ���� ������������ �������, ��� ��� ��������� � ��������� ������� ����� ������, � ������ ���������� ��������� 
	//����� �������
	//unbind
	glBindVertexArray(0);
}

void DebugRenderer::end(){

}

void DebugRenderer::drawBox(const glm::vec4& destRect, const Color color, float angle){

}

void DebugRenderer::drawCircle(const glm::vec2& center, const Color color, float radius){

}

void DebugRenderer::render(){

}
//opposite of init 
void DebugRenderer::dispose(){

}

}
