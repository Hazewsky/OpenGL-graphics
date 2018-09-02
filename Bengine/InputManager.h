#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
namespace Bengine{
	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void update();

		//����� enum ����� ��� ������
		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);
		//true  ���� ������ ������
		bool isKeyPressed(unsigned int keyID);
		//true ���� ������ ������
		bool isKeyDown(unsigned int keyID);
		void setMouseCoords(float x, float y);
		
		//get
		//const ����� ��� �� ������ ������ ������ ������, �� ������
		//������ � ����� ������������ const
		glm::vec2 getMouseCoords() const{
			return _mouseCoords;
		}
	private:
		bool wasKeyDown(unsigned int keyID);
		//������ ������ ������ ��� �������. ������ ������� �������� ������ � ���. ����� ������������ ������ ������
		//� �� ������ ������ �� �������, � ������� �� �������� ����
		//Hash table
		//Underlying array has constant size to store 128 elements and each slot contains key - value pair.
		//Key is stored to distinguish between key - value pairs, which have the same hash.
		//Hash function
		//Table allows only integers as values.Hash function to be used is the remainder of division by 128.
		//In the view of implementation, this hash function can be encoded using remainder operator or using bitwise AND with 127.
		//Issues
		//When there is no more place in the table, the loop, searching for empty slot, will run infinitely. 
		//It won't happen in real hash table based on open addressing, because it is most likely dynamic-sized.
		
		
		//��� 1 - ����� �������, 2 - ������ �� ���

		std::unordered_map<unsigned int, bool> _keyMap;
		std::unordered_map<unsigned int, bool> _previousKeyMap;

		glm::vec2 _mouseCoords;

	};

}