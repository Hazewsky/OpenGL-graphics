#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "Vertex.h"

namespace Bengine{

	enum class GlyphSortType{
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE //��� �������� ������ ������������ ������. � �������� �����, ��� ����� �����������
	};
//��� 1 ������, � ���. 6 �����, ��� ������� ������� ��� ����� ���. , depth(�������) ��� ������������� �����������
class Glyph{
public:
	Glyph(){}
	Glyph(const glm::vec4& DestRect, const glm::vec4& UVRect, GLuint Texture, float Depth, const Color& Color, float angle);
	Glyph(const glm::vec4& DestRect, const glm::vec4& UVRect, GLuint Texture, float Depth, const Color& Color);
	GLuint texture;
	float depth; 

	Vertex topLeft;
	Vertex bottomLeft;
	Vertex topRight;
	Vertex bottomRight;
private:
	glm::vec2 rotatePoint(glm::vec2, float angle);
};

class RenderBatch{
	//������ ���������� ��� ���������, ������� offset _vbo
public:
	RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : 
		offset(Offset),
		numVertices(NumVertices), 
		texture(Texture)
	{
	}
	GLuint offset;
	//������� ����� ��������
	GLuint numVertices;
	GLuint texture;
};

class SpriteBatch
{
public:
	SpriteBatch();
	~SpriteBatch();
	//init - begin - draw(������ ������� � ����) - end(��������������, ����������,�������) - render(��� ��� ��� ���������) 
	
	void init();

	void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);

	void end();
	//z as width, w as height
	//destination Rectangle && uv coords (bottom left corner && dimensions)
	//������ �������� � �� ����
	//�� ���������� �����, ����� �� �������� ��������.
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color);
	//���������� ����
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color, float angle);
	//������������ ������������ ������
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color, const glm::vec2& dir);
	void renderBatch();
private:
	void createRenderBatches();
	void createVertexArray();
	void sortGlyphs();

	static bool compareFrontToBack(Glyph * a, Glyph * b);
	static bool compareBackToFront(Glyph * a, Glyph * b);
	static bool compareTexture(Glyph * a, Glyph * b);

	GLuint _vbo;
	GLuint _vao;

	GlyphSortType _sortType;
	//��� ������� ������ ������, �� �� ����� ����� ��� ��� �������
	//������ new b delete �� ����� ������ ������ push_back
	//�� ����� ������ ������ ����� clear, ��� ������ �� ��������� ������
	std::vector <Glyph> _glyphs;///< ��� ��� ���� �����
	//� ��� ��� ����� ��� ���������� ������
	//��� ����� ��������� � ������ �� ������
	//��������� ��������� �� ������ ������(��� ������ Glyph � _glyphs
	//��� ��-�� ���� ��� ��� ���������� ��� �������� ���������������, ��������������
	//�������� � ����� ������ ��������, ��� ����� ��� �������� ��������� �� ������(��� � Glyph*)
	//4||8 bytes per each pointer
	std::vector<Glyph *> _glyphPointers;///< � ��� ��� ����������
	std::vector<RenderBatch> _renderBatches;
};

}
