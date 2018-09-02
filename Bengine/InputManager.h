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

		//нужен enum класс дл€ клавиш
		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);
		//true  если кнопка нажата
		bool isKeyPressed(unsigned int keyID);
		//true если кнопка зажата
		bool isKeyDown(unsigned int keyID);
		void setMouseCoords(float x, float y);
		
		//get
		//const чтобы она не мен€ла ничего внутри класса, на вс€кий
		//обычно в гетах используетс€ const
		glm::vec2 getMouseCoords() const{
			return _mouseCoords;
		}
	private:
		bool wasKeyDown(unsigned int keyID);
		//вместо дерева хранит хеш таблицу. —легка быстрее получает доступ к ней. ћожет использовать больше пам€ти
		//и не хранит данные по пор€дку, в отличии от обычного мапа
		//Hash table
		//Underlying array has constant size to store 128 elements and each slot contains key - value pair.
		//Key is stored to distinguish between key - value pairs, which have the same hash.
		//Hash function
		//Table allows only integers as values.Hash function to be used is the remainder of division by 128.
		//In the view of implementation, this hash function can be encoded using remainder operator or using bitwise AND with 127.
		//Issues
		//When there is no more place in the table, the loop, searching for empty slot, will run infinitely. 
		//It won't happen in real hash table based on open addressing, because it is most likely dynamic-sized.
		
		
		//тут 1 - номер клавиши, 2 - нажата ли она

		std::unordered_map<unsigned int, bool> _keyMap;
		std::unordered_map<unsigned int, bool> _previousKeyMap;

		glm::vec2 _mouseCoords;

	};

}