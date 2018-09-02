#include "IOManager.h"
#include <fstream>
namespace Bengine{
	bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char> & buffer){
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail()){
			perror(filePath.c_str());
			return false;
		}

		file.seekg(0, std::ios::end);

		//get the file size
		int fileSize = file.tellg();

		file.seekg(0, std::ios::beg);
		//disregard header in file
		fileSize -= file.tellg();

		buffer.resize(fileSize);
		//get c_str from vector
		file.read((char *)&(buffer[0]), fileSize);
		file.close();
		return true;
	}

	bool IOManager::readFileToBuffer(std::string filePath, std::string& buffer){
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail()){
			perror(filePath.c_str());
			return false;
		}

		file.seekg(0, std::ios::end);

		//get the file size
		int fileSize = file.tellg();

		file.seekg(0, std::ios::beg);
		//disregard header in file
		fileSize -= file.tellg();

		buffer.resize(fileSize);
		//get c_str from vector
		file.read((char *)&(buffer[0]), fileSize);
		file.close();
		return true;
	}
}