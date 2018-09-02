#include "GLSLProgram.h"
#include "BengineErrors.h"
#include "IOManager.h"
#include <fstream>
#include <vector>
namespace Bengine{
	GLSLProgram::GLSLProgram() : _numAttributes(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0) //initialization list
	{

	}


	GLSLProgram::~GLSLProgram()
	{
	}
	void GLSLProgram::compileShaders(const std::string vertexShaderFilePath, const std::string fragmentShaderFilePath){
		std::string vertSource;
		std::string fragSource;

		IOManager::readFileToBuffer(vertexShaderFilePath, vertSource);
		IOManager::readFileToBuffer(fragmentShaderFilePath, fragSource);
		//Vertex and fragment shaders are successfully compiled.
		//Now time to link them together into a program.
		//Get a program object/
		_programID = glCreateProgram();

		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

		if (_vertexShaderID == 0){
			fatalError("Vertex shader failed to be created");
		}

		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (_fragmentShaderID == 0){
			fatalError("Fragment shader failed to be created");
		}
		
		compileShader(vertSource.c_str(), vertexShaderFilePath,_vertexShaderID);
		compileShader(fragSource.c_str(), fragmentShaderFilePath, _fragmentShaderID);

	}

	void GLSLProgram::linkShaders(){


		//Attach out shaders to out program
		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);

		//link out program
		glLinkProgram(_programID);

		//Note the different functions here:glGetProgram* instead of glGetShader*
		GLint isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE){
			GLint maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);
			//The maxLength includer the NULL character
			/*no strings allowed :( */ std::vector<GLchar>errorLog(maxLength);
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);


			//we don't need the program anymore
			glDeleteProgram(_programID);
			//Don't leak the shaders either
			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);
			//use the infoLog as you see fit.

			std::printf("%s\n", &(errorLog[0]));//like the string
			fatalError("Shaders failed to link");

		}
		//always detach shaders after a successful link.
		glDetachShader(_programID, _vertexShaderID);
		glDetachShader(_programID, _fragmentShaderID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
	}

	void GLSLProgram::addAttribute(const std::string& attributeName){
		glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());

	}

	GLint GLSLProgram::getUniformLocation(const std::string& uniformName){
		GLint location = glGetUniformLocation(_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX){ //will return if object doesn't exist
			fatalError("Uniform " + uniformName + " not found in shader");
		}
		return location;

	}
	//enable the shader and all its attributes
	void GLSLProgram::use(){
		glUseProgram(_programID);
		for (int i = 0; i < _numAttributes; i++){
			glEnableVertexAttribArray(i); //need to enable every attribute
		}
	}
	//disable the shader and all its attributes
	void GLSLProgram::unuse(){
		glUseProgram(0);
		for (int i = 0; i < _numAttributes; i++){
			glDisableVertexAttribArray(i); //need to disable every attribute
		}
	}

	void GLSLProgram::compileShader(const char* source, std::string name, GLuint id){
		/*std::ifstream vertexFile(source);
		if (vertexFile.fail()){
			perror(filePath.c_str());
			fatalError("Failed to open " + source);
		}

		std::string fileContents = "";
		std::string line;
		while (std::getline(vertexFile, line)){
			fileContents += line + "\n";
		}
		vertexFile.close(); */
		//const char* contentsPtr = fileContents.c_str();//если просто в нижнюю пвихнуть &fileContents.c_str(), будет ошибка
		//а так вполне легально, хули. Так то функция хочет массив строк
		glShaderSource(id, 1, &source, nullptr);
		glCompileShader(id);

		GLint success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);//проверка на правильность кода в файле

		if (success == GL_FALSE){
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
			//The maxLength includer the NULL character
			/*no strings allowed :( */ std::vector<char>errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
			//Provide the infolog in whatever manor you deem best
			//Exit with failure.
			glDeleteShader(id);//Don't leak the shader
			std::printf("%s\n", &(errorLog[0]));//like the string
			fatalError("Shader failed to compile");
		}
	}
	void GLSLProgram::compileShadersFromSource(const char* /*==string.c_str*/ vertexSource, const char* fragmentSource){

	}
}