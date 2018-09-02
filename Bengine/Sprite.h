#pragma once
#include <GL/glew.h>
#include "GLTexture.h"
#include <string>
namespace Bengine{
	class Sprite{
	public:
		Sprite();
		~Sprite();

		void init(float x, float y, float width, float height, std::string texturePath);

		void draw();

	private:
		float _x, _y, _width, _height;
		GLuint _vboID; //unsigned int/ Guaranteeed 32 bits/ Vertex Buffer Objects
		GLTexture _texture;
	};
}
