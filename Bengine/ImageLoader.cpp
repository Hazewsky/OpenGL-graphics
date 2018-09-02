#include "ImageLoader.h"
#include "picoPNG.h"
#include "IOManager.h"
#include "BengineErrors.h"
namespace Bengine{
	GLTexture ImageLoader::loadPNG(std::string filePath){
		GLTexture texture = {};//инициализация каждой переменной в структуре == 0
		std::vector<unsigned char>in;
		std::vector<unsigned char> out;

		unsigned long width, height;

		if (IOManager::readFileToBuffer(filePath, in) == false){
			fatalError("failed to load PNG file to buffer");
		}

		int errorCode = decodePNG(out, width, height, &(in[0]), in.size(), true);
		if (errorCode != 0){
			fatalError("decodePNG with error: " + std::to_string(errorCode));
		}

		glGenTextures(1, &(texture.id));

		glBindTexture(GL_TEXTURE_2D, texture.id); //bind the texture

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//target(2d,3d), how to wrap image, 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//mip mapping. 3rd is how to filter image. Do linear intrapolation
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);//unbind

		texture.width = width;
		texture.height = height;
		return texture;
	}

}