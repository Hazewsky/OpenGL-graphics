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
		TEXTURE //все текстуры должны отрисоваться вместе. В основном изаем, ибо самый эффективный
	};
//это 1 спрайт, и все. 6 точек, тип тестуры который оно будет исп. , depth(глубина) для многослойного отображения
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
	//хранит информацию как рендерить, хранить offset _vbo
public:
	RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : 
		offset(Offset),
		numVertices(NumVertices), 
		texture(Texture)
	{
	}
	GLuint offset;
	//сколько точек отрисуем
	GLuint numVertices;
	GLuint texture;
};

class SpriteBatch
{
public:
	SpriteBatch();
	~SpriteBatch();
	//init - begin - draw(только добавит в батч) - end(постпроцессинг, сортировка,дпустим) - render(вот это уже отрисовка) 
	
	void init();

	void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);

	void end();
	//z as width, w as height
	//destination Rectangle && uv coords (bottom left corner && dimensions)
	//дальше текстура и ее цвет
	//мы используем конст, чтобы не изменять оригинал.
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color);
	//использует угол
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color, float angle);
	//используется направляющий вектор
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
	//так юзается дохера памяти, но мы можем легко все это удалить
	//вместо new b delete мы можем просто юзнуть push_back
	//но когда чистим вектор через clear, оно нихера не освободит память
	std::vector <Glyph> _glyphs;///< вот это сами глифы
	//а вот это юзаем для сортировки данных
	//тут будет указатель к одному из глифов
	//указатель указывает на массив памяти(ибо просто Glyph в _glyphs
	//Все из-за того что без указателей оно хранится последовательно, соответственно
	//Работать с такой херней приятнее, чем когда все случайно раскидано по памяти(как с Glyph*)
	//4||8 bytes per each pointer
	std::vector<Glyph *> _glyphPointers;///< а это для сортировки
	std::vector<RenderBatch> _renderBatches;
};

}
