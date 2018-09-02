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
	//иниц. шейдеров

	//устанавливаем буфферы
	//количество, массив
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	//бидним
	glBindVertexArray(m_vao);
	//даем буффер
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	//всегда надо отбиндить вертект ареей.
	//нам не надо отбайндивать буфферы, ибо они забиндены в контексте вертект аррей объект, и потому анбиндятся автоматом 
	//после объекта
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
