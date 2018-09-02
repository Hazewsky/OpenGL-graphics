#pragma once
#include <string> 
#include <GL/glew.h>

namespace Bengine{
	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		void compileShaders(const std::string vertexShaderFilePath, const std::string fragmentShaderFilePath);

		void compileShadersFromSource(const char* /*==string.c_str*/ vertexSource, const char* fragmentSource);
		void linkShaders();

		void addAttribute(const std::string& attributeName);

		GLint getUniformLocation(const std::string& uniformName);

		void use();

		void unuse();
	private:
		int _numAttributes;

		GLuint _programID;

		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;

		void compileShader(const char* source, std::string name, GLuint id);
		
	};

}