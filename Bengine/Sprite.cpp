#include "Sprite.h"
#include "Vertex.h"
#include <stddef.h>
#include <Windows.h>
#include "ResourceManager.h"

//using namespace Bengine; ﬁ«¿… ›“Œ ÀŒÀ

namespace Bengine{
	Sprite::Sprite()
	{
		_vboID = 0;//always to 0
	}


	Sprite::~Sprite()
	{
		//free the buffer
		if (_vboID != 0){
			glDeleteBuffers(1, &_vboID);
		}
	}


	void Sprite::init(float x, float y, float width, float height, std::string texturePath){
		_x = x;
		_y = y;
		_width = width;
		_height = height;

		_texture = ResourceManager::getTexture(texturePath);

		//Generate the buffer if it hasn't already been generated
		if (_vboID == 0){
			glGenBuffers(1, &_vboID);
		}
		//float vertexData[12];// all x,y,z are always float
		////first triangle
		//vertexData[0] = x + width;
		//vertexData[1] = y + width;
		//vertexData[2] = x;
		//vertexData[3] = y + height;
		//vertexData[4] = x;
		//vertexData[5] = y;
		////second triangle
		//vertexData[6] = x ;
		//vertexData[7] = y;
		//vertexData[8] = x +width;
		//vertexData[9] = y;
		//vertexData[10] = x+width;
		//vertexData[11] = y+height;

		Vertex vertexData[6];

		vertexData[0].setPosition(x + width, y + height);//x,y
		vertexData[0].setUV(1.0f, 1.0f);

		vertexData[1].setPosition(x, y + height);
		vertexData[1].setUV(0.0f, 1.0f);

		vertexData[2].setPosition(x, y);
		vertexData[2].setUV(0.0f, 0.0f);

		//second triangle

		vertexData[3].setPosition(x, y);
		vertexData[3].setUV(0.0f, 0.0f);

		vertexData[4].setPosition(x + width, y);
		vertexData[4].setUV(1.0f, 0.0f);

		vertexData[5].setPosition(x + width, y + height);
		vertexData[5].setUV(1.0f, 1.0f);


		for (int i = 0; i < 6; i++){
			vertexData[i].setColor(255, 0, 255, 255);
		}
		vertexData[1].setColor(0, 255, 0, 255);


		vertexData[4].setColor(0, 0, 0, 255);




		glBindBuffer(GL_ARRAY_BUFFER, _vboID);//bind buffer, array buf is basic
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW); //upload buffer

		glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind buffer
	}

	void Sprite::draw(){

		glBindTexture(GL_TEXTURE_2D, _texture.id);//no need to unbind, cuz it may be another texture that uses another texture

		glBindBuffer(GL_ARRAY_BUFFER, _vboID);//active buffer

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
		//Draw 6 verticles to the screen
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(0);//disable
		glDisableVertexAttribArray(1);//disable
		glDisableVertexAttribArray(2);//disable

		glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind buffer
	}




}