#include "SpriteBatch.h"
#include <algorithm>

namespace Bengine{
	
	Glyph::Glyph(const glm::vec4& DestRect, const glm::vec4& UVRect, GLuint Texture, float Depth, const Color& Color) :
		//слегка быстрее через лист
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

		//внизу, + rect.w[3]( высота экрана) не надо. Зато надо +destRect.z[2](ширина экрана), ибо он справа
		bottomRight.color = Color;
		bottomRight.setPosition(DestRect.x + DestRect.z, DestRect.y);
		bottomRight.setUV(UVRect.x + UVRect.z, UVRect.y);

		//тут надо и w, т.к. вверху, и z,  т.к. справа
		topRight.color = Color;
		topRight.setPosition(DestRect.x + DestRect.z, DestRect.y + DestRect.w);
		topRight.setUV(UVRect.x + UVRect.z, UVRect.y + UVRect.w);

	}

	Glyph::Glyph(const glm::vec4& DestRect, const glm::vec4& UVRect, GLuint Texture, float Depth, const Color& Color, float angle) :
		//слегка быстрее через лист
		texture(Texture),
		depth(Depth)
	{
		//схема такая. Берется агент, ставится в исходную позицию(0,0), крутится и возвращается назда
		//делит высоту и ширину на 2
		glm::vec2 halfDims(DestRect.z / 2.0f, DestRect.w / 2.0f);
		//эта штука представляет спрайт в центре origin'a(0.0, 0.0 на координатной оси)
		//левый верхний угол
			glm::vec2 tl(-halfDims.x, halfDims.y);
			//левый нижний
			glm::vec2 bl(-halfDims.x, -halfDims.y);
			//правый верхний
			glm::vec2 tr(halfDims.x, halfDims.y);
			//правый нижний
			glm::vec2 br(halfDims.x, -halfDims.y);

			//поворачиваем точки и возвращаем назад из origin'a(дальше)
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

		//внизу, + rect.w[3]( высота экрана) не надо. Зато надо +destRect.z[2](ширина экрана), ибо он справа
		bottomRight.color = Color;
		bottomRight.setPosition(DestRect.x + br.x, DestRect.y + br.y);
		bottomRight.setUV(UVRect.x + UVRect.z, UVRect.y);

		//тут надо и w, т.к. вверху, и z,  т.к. справа
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
		//Установит все состояние что нам нужны чтобы начать рендеринг
		//Установить как мы хотим отсортировать
		_sortType = sortType;
		//Чистим вектор, ибо будем добавлять и добавлять новые точки, и overflow нам нах не сдался
		//меняем размер снова на 0. и все
		_renderBatches.clear();

		_glyphs.clear();
	}

	void SpriteBatch::end(){
		//размер вектора указателей
		_glyphPointers.resize(_glyphs.size());
		//set up все указатели для быстрой сортировки
		for (int i = 0; i < _glyphs.size(); i++){
			_glyphPointers[i] = &_glyphs[i];
		}
		//тут нам надо отсортировать глифы
		// и сгенерировать батчи для глифов(какие глифы должны сгенерироваться вместе макс. эффективным способом)
		sortGlyphs();
		createRenderBatches();

	}
	//z as width, w as height
	//destination Rectangle && uv coords (bottom left corner && dimensions)
	//дальше текстура и ее цвет
	//мы используем конст, чтобы не изменять оригинал.
	void SpriteBatch::draw(const glm::vec4& destRect,
							const glm::vec4& uvRect,
							GLuint texture,
							float depth,
							const Color& color){
		
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
		
	}


	//использует угол
	void SpriteBatch::draw(const glm::vec4& destRect,
							const glm::vec4& uvRect,
							GLuint texture, 
							float depth, 
							const Color& color,
							float angle){
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}
	//используется направляющий вектор
	void SpriteBatch::draw(const glm::vec4& destRect, 
							const glm::vec4& uvRect,
							GLuint texture,
							float depth,
							const Color& color,
							const glm::vec2& dir){
		//источник(дефолтный угол)направо!
		//они уже нормализованные, unit вектора
		const glm::vec2 right(1.0f, 0.0f);
		//что такое dot? это произведение, по формуле v1.x * v2.x + v1.y * v2.y
		//всегда должно быть положительным. Если отрицательное - значит поворачивает по часовой ( а мы всегда делаем против !)
		float angle = acos(glm::dot(right, dir));
		if (dir.y < 0){
			angle = -angle;
		}
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);

	}
	void SpriteBatch::renderBatch(){
		glBindVertexArray(_vao);
				//пройдет через все _renderBatches и нарисует их все
		for (int i = 0; i < _renderBatches.size(); i++){
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
			//тип, начало, количество
			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
		}
		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches(){
		//пройти через все глифы и добавить к батчу.
		//если есть новый глиф, добавить новый батч

		std::vector<Vertex> vertices;
		//чтобы он постоянно не увеличивал размер при добавлении. так будет быстрее. резервируем память под 6 точек на каждый глиф
		vertices.resize(_glyphPointers.size() * 6);
		//reserve просто выделяет память, но сам размер вектора будет 0, т.к. пуст. При resize изменяется размер
		//проверяем есть ли вообще глифы
		if (_glyphPointers.empty()){
			return;
		}
		int offset = 0;
		int cv = 0; //current vertex

		//создаем первый батч, используя первый глиф
		//0 - первый, 6 мы используем всегда, т.к. два треугольника, 
		//так делает копию и сразу выкидывает myBatch(оригинал скорее всего)
		//RenderBatch myBatch(0, 6, _glyphPointers[0]->texture);
		//	_renderBatches.push_back(myBatch);
		//так вместо впиндуривания myBatch, впиндрюиваем parameter list(в скобках) конструктора
		//создаст новый объект и инициализирует без мгновенной копии
		_renderBatches.emplace_back(0, 6, _glyphPointers[0]->texture);
		//инкрементирует после исполнения строки
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

		// cg - current glyph. 1 - потому что 0, первый тоесть, мы уже запилили
		for (int cg = 1; cg < _glyphPointers.size(); cg++){
			if (_glyphPointers[cg]->texture != _glyphPointers[cg - 1]->texture){
				//если текстура другая, тогда впиндюрим новый батч
				_renderBatches.emplace_back(offset, 6, _glyphPointers[cg]->texture);
			}
			else{
				//иначе увеличиваем размер. back - крайний элемент
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
		//теперь загружаем 
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//2 - размер 3 - адрес первого элемента, 4 - т.к. грузим каждый кадр
		//stream - если биндим, используем и выкидываем
		//dynamic - если мы вносим в него изменения
		//но тут мы перезаписываем
		//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
		//А так выкидываем старое. Называется orphan the buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		//Теперь грузим данные
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
		//ТАк быстрее

		//разбиндиваем к херам
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//vertices удаляется автоматом, ибо статик
	}
	void SpriteBatch::createVertexArray(){
		//инкапсулирует все из draw. Трабл в том, что вся так шняга из sprite.h draw
		//вызывается кучу раз и слегка тормозит игру. Теперь мы вызовем это один разок тут
		
		
		//генерим только если их еще нет(==0). Вдруг кто-то рискнет сделать инициализацию дважды
		if (_vao == 0){
			//генерим 1 и указатель на вао
			glGenVertexArrays(1, &_vao);
		}
		//Если мы меняем GLState в буффере, оно будет храниться здесь, VertexArrayObject
		glBindVertexArray(_vao);
		
		if (_vbo == 0){
			//1 буффер указателя на  вбо
			glGenBuffers(1, &_vbo);
		}
		//Теперь каждый раз как мы будем ребайндить вертех аррей, оно автоматом забайндит буффер.
		//Таким образом В draw нам больше байндить его не надо
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		

		//Эта фигня установить все как мы укажем, ибо мы юзаем 1 шейдер на все, каждый раз как мы enabl'им _vao
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

		//теперь анбайндим нах
		glBindVertexArray(0);
	}

	void SpriteBatch::sortGlyphs(){
		//stable_sort гарантирует что два одинаковых элемента сохранят свой(один и тот же) порядок.
		//Если надо нарисовать один элемент перед другим, но у них будет 1 текстура, то все будет норм
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
	//чтобы не юзать больше батчей чем нам надо. сравнивает id
	bool  SpriteBatch::compareTexture(Glyph * a, Glyph * b){
		return (a->texture < b->texture);
	}
}