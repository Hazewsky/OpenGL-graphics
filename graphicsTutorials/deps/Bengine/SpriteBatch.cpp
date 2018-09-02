#include "SpriteBatch.h"
#include <algorithm>

namespace Bengine{
	
	Glyph::Glyph(const glm::vec4& DestRect, const glm::vec4& UVRect, GLuint Texture, float Depth, const Color& Color) :
		//������ ������� ����� ����
		texture(Texture),
		depth(Depth)
	{
		topLeft.color = Color;
		topLeft.setPosition(DestRect.x, DestRect.y + DestRect.w);
		topLeft.setUV(UVRect.x, UVRect.y + UVRect.w);
		//x==[0], y==[1]
		bottomLeft.color = Color;
		bottomLeft.setPosition(DestRect.x, DestRect.y);
		bottomLeft.setUV(UVRect.x, UVRect.y);

		//�����, + rect.w[3]( ������ ������) �� ����. ���� ���� +destRect.z[2](������ ������), ��� �� ������
		bottomRight.color = Color;
		bottomRight.setPosition(DestRect.x + DestRect.z, DestRect.y);
		bottomRight.setUV(UVRect.x + UVRect.z, UVRect.y);

		//��� ���� � w, �.�. ������, � z,  �.�. ������
		topRight.color = Color;
		topRight.setPosition(DestRect.x + DestRect.z, DestRect.y + DestRect.w);
		topRight.setUV(UVRect.x + UVRect.z, UVRect.y + UVRect.w);

	}

	Glyph::Glyph(const glm::vec4& DestRect, const glm::vec4& UVRect, GLuint Texture, float Depth, const Color& Color, float angle) :
		//������ ������� ����� ����
		texture(Texture),
		depth(Depth)
	{
		//����� �����. ������� �����, �������� � �������� �������(0,0), �������� � ������������ �����
		//����� ������ � ������ �� 2
		glm::vec2 halfDims(DestRect.z / 2.0f, DestRect.w / 2.0f);
		//��� ����� ������������ ������ � ������ origin'a(0.0, 0.0 �� ������������ ���)
		//����� ������� ����
			glm::vec2 tl(-halfDims.x, halfDims.y);
			//����� ������
			glm::vec2 bl(-halfDims.x, -halfDims.y);
			//������ �������
			glm::vec2 tr(halfDims.x, halfDims.y);
			//������ ������
			glm::vec2 br(halfDims.x, -halfDims.y);

			//������������ ����� � ���������� ����� �� origin'a(������)
			tl = rotatePoint(tl, angle) + halfDims;
			bl = rotatePoint(bl, angle) + halfDims;

			tr = rotatePoint(tr, angle) + halfDims;
			br = rotatePoint(br, angle) + halfDims;


		topLeft.color = Color;
		topLeft.setPosition(DestRect.x + tl.x, DestRect.y + tl.y);
		topLeft.setUV(UVRect.x, UVRect.y + UVRect.w);
		//x==[0], y==[1]
		bottomLeft.color = Color;
		bottomLeft.setPosition(DestRect.x + bl.x, DestRect.y + bl.y);
		bottomLeft.setUV(UVRect.x, UVRect.y);

		//�����, + rect.w[3]( ������ ������) �� ����. ���� ���� +destRect.z[2](������ ������), ��� �� ������
		bottomRight.color = Color;
		bottomRight.setPosition(DestRect.x + br.x, DestRect.y + br.y);
		bottomRight.setUV(UVRect.x + UVRect.z, UVRect.y);

		//��� ���� � w, �.�. ������, � z,  �.�. ������
		topRight.color = Color;
		topRight.setPosition(DestRect.x + tr.x, DestRect.y + tr.y);
		topRight.setUV(UVRect.x + UVRect.z, UVRect.y + UVRect.w);

	}

	glm::vec2 Glyph::rotatePoint(glm::vec2 pos, float angle){
		glm::vec2 newV;
		newV.x = pos.x * cos(angle) - pos.y * sin(angle);
		newV.y = pos.x * sin(angle) + pos.y * cos(angle);
		return newV;
	}



	SpriteBatch::SpriteBatch() : _vbo(0), _vao(0)
	{
	}
	SpriteBatch::~SpriteBatch()
	{
	}




	void SpriteBatch::init(){
		createVertexArray();
	}

	void SpriteBatch::begin(GlyphSortType sortType/* = GlyphSortType::TEXTURE */){
		//��������� ��� ��������� ��� ��� ����� ����� ������ ���������
		//���������� ��� �� ����� �������������
		_sortType = sortType;
		//������ ������, ��� ����� ��������� � ��������� ����� �����, � overflow ��� ��� �� ������
		//������ ������ ����� �� 0. � ���
		_renderBatches.clear();

		_glyphs.clear();
	}

	void SpriteBatch::end(){
		//������ ������� ����������
		_glyphPointers.resize(_glyphs.size());
		//set up ��� ��������� ��� ������� ����������
		for (int i = 0; i < _glyphs.size(); i++){
			_glyphPointers[i] = &_glyphs[i];
		}
		//��� ��� ���� ������������� �����
		// � ������������� ����� ��� ������(����� ����� ������ ��������������� ������ ����. ����������� ��������)
		sortGlyphs();
		createRenderBatches();

	}
	//z as width, w as height
	//destination Rectangle && uv coords (bottom left corner && dimensions)
	//������ �������� � �� ����
	//�� ���������� �����, ����� �� �������� ��������.
	void SpriteBatch::draw(const glm::vec4& destRect,
							const glm::vec4& uvRect,
							GLuint texture,
							float depth,
							const Color& color){
		
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
		
	}


	//���������� ����
	void SpriteBatch::draw(const glm::vec4& destRect,
							const glm::vec4& uvRect,
							GLuint texture, 
							float depth, 
							const Color& color,
							float angle){
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}
	//������������ ������������ ������
	void SpriteBatch::draw(const glm::vec4& destRect, 
							const glm::vec4& uvRect,
							GLuint texture,
							float depth,
							const Color& color,
							const glm::vec2& dir){
		//��������(��������� ����)�������!
		//��� ��� ���������������, unit �������
		const glm::vec2 right(1.0f, 0.0f);
		//��� ����� dot? ��� ������������, �� ������� v1.x * v2.x + v1.y * v2.y
		//������ ������ ���� �������������. ���� ������������� - ������ ������������ �� ������� ( � �� ������ ������ ������ !)
		float angle = acos(glm::dot(right, dir));
		if (dir.y < 0){
			angle = -angle;
		}
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);

	}
	void SpriteBatch::renderBatch(){
		glBindVertexArray(_vao);
				//������� ����� ��� _renderBatches � �������� �� ���
		for (int i = 0; i < _renderBatches.size(); i++){
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
			//���, ������, ����������
			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
		}
		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches(){
		//������ ����� ��� ����� � �������� � �����.
		//���� ���� ����� ����, �������� ����� ����

		std::vector<Vertex> vertices;
		//����� �� ��������� �� ���������� ������ ��� ����������. ��� ����� �������. ����������� ������ ��� 6 ����� �� ������ ����
		vertices.resize(_glyphPointers.size() * 6);
		//reserve ������ �������� ������, �� ��� ������ ������� ����� 0, �.�. ����. ��� resize ���������� ������
		//��������� ���� �� ������ �����
		if (_glyphPointers.empty()){
			return;
		}
		int offset = 0;
		int cv = 0; //current vertex

		//������� ������ ����, ��������� ������ ����
		//0 - ������, 6 �� ���������� ������, �.�. ��� ������������, 
		//��� ������ ����� � ����� ���������� myBatch(�������� ������ �����)
		//RenderBatch myBatch(0, 6, _glyphPointers[0]->texture);
		//	_renderBatches.push_back(myBatch);
		//��� ������ ������������� myBatch, ������������ parameter list(� �������) ������������
		//������� ����� ������ � �������������� ��� ���������� �����
		_renderBatches.emplace_back(0, 6, _glyphPointers[0]->texture);
		//�������������� ����� ���������� ������
		// topleft[0][5]		*--*	topright[4]						*\   * *
		//						|  |									* *	  \*
		// bottomleft[1]		*--*	bottomright[2][3]
		vertices[cv++] = _glyphPointers[0]->topLeft;
		vertices[cv++] = _glyphPointers[0]->bottomLeft;
		vertices[cv++] = _glyphPointers[0]->bottomRight;
		vertices[cv++] = _glyphPointers[0]->bottomRight;
		vertices[cv++] = _glyphPointers[0]->topRight;
		vertices[cv++] = _glyphPointers[0]->topLeft;
		offset += 6;

		// cg - current glyph. 1 - ������ ��� 0, ������ ������, �� ��� ��������
		for (int cg = 1; cg < _glyphPointers.size(); cg++){
			if (_glyphPointers[cg]->texture != _glyphPointers[cg - 1]->texture){
				//���� �������� ������, ����� ��������� ����� ����
				_renderBatches.emplace_back(offset, 6, _glyphPointers[cg]->texture);
			}
			else{
				//����� ����������� ������. back - ������� �������
				_renderBatches.back().numVertices += 6;
			}
			vertices[cv++] = _glyphPointers[cg]->topLeft;
			vertices[cv++] = _glyphPointers[cg]->bottomLeft;
			vertices[cv++] = _glyphPointers[cg]->bottomRight;
			vertices[cv++] = _glyphPointers[cg]->bottomRight;
			vertices[cv++] = _glyphPointers[cg]->topRight;
			vertices[cv++] = _glyphPointers[cg]->topLeft;
			offset += 6;
		}
		//������ ��������� 
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//2 - ������ 3 - ����� ������� ��������, 4 - �.�. ������ ������ ����
		//stream - ���� ������, ���������� � ����������
		//dynamic - ���� �� ������ � ���� ���������
		//�� ��� �� ��������������
		//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
		//� ��� ���������� ������. ���������� orphan the buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		//������ ������ ������
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
		//��� �������

		//������������ � �����
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//vertices ��������� ���������, ��� ������
	}
	void SpriteBatch::createVertexArray(){
		//������������� ��� �� draw. ����� � ���, ��� ��� ��� ����� �� sprite.h draw
		//���������� ���� ��� � ������ �������� ����. ������ �� ������� ��� ���� ����� ���
		
		
		//������� ������ ���� �� ��� ���(==0). ����� ���-�� ������� ������� ������������� ������
		if (_vao == 0){
			//������� 1 � ��������� �� ���
			glGenVertexArrays(1, &_vao);
		}
		//���� �� ������ GLState � �������, ��� ����� ��������� �����, VertexArrayObject
		glBindVertexArray(_vao);
		
		if (_vbo == 0){
			//1 ������ ��������� ��  ���
			glGenBuffers(1, &_vbo);
		}
		//������ ������ ��� ��� �� ����� ���������� ������ �����, ��� ��������� ��������� ������.
		//����� ������� � draw ��� ������ �������� ��� �� ����
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		

		//��� ����� ���������� ��� ��� �� ������, ��� �� ����� 1 ������ �� ���, ������ ��� ��� �� enabl'�� _vao
		glEnableVertexAttribArray(0);//using very 1st elements, we need only 1 array
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		//this is the posotion attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		//where to start. 0 - like init. 2 - x&&y. GL_FLOAT - type of vertexData, GL_FALSE - is normalized? (convert to range between 0 and 1)
		// size of struct, 0 start pointer(memory),from which byte must we start

		//This is color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		//1 because it's next vector
		//GL_TRUE will convert 0-255 to 0-1
		//This is UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		//������ ��������� ���
		glBindVertexArray(0);
	}

	void SpriteBatch::sortGlyphs(){
		//stable_sort ����������� ��� ��� ���������� �������� �������� ����(���� � ��� ��) �������.
		//���� ���� ���������� ���� ������� ����� ������, �� � ��� ����� 1 ��������, �� ��� ����� ����
		switch (_sortType){
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareBackToFront);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareFrontToBack);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareTexture);
			break;
		}
	}

	bool  SpriteBatch::compareFrontToBack(Glyph * a, Glyph * b){
		return (a->depth < b->depth);
	}

	bool  SpriteBatch::compareBackToFront(Glyph * a, Glyph * b){
		return (a->depth > b->depth);
	}
	//����� �� ����� ������ ������ ��� ��� ����. ���������� id
	bool  SpriteBatch::compareTexture(Glyph * a, Glyph * b){
		return (a->texture < b->texture);
	}
}