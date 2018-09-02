#include "InputManager.h"

namespace Bengine{
	//все поля _mouseCoords будут равны 0.0f
	InputManager::InputManager() : _mouseCoords(0.0f)
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::update(){
		for (auto &it : _keyMap){
			//проход через все элементы текущего мапа и захуяривание их в предыдущий
			_previousKeyMap[it.first] = it.second;
		}
	}


	void InputManager::pressKey(unsigned int keyID){
		//если keyID не будет - создаст
		_keyMap[keyID] = true;
	}

	void  InputManager::releaseKey(unsigned int keyID){
		_keyMap[keyID] = false;
	}


	bool  InputManager::isKeyDown(unsigned int keyID){
		//При такой записи, если ничего не было инициализировано(нажато), то оно создаст ассоциацию по умолчанию.
		//Случайный булеан
		//Это пиздец как нехорошо
		//return _keyMap[keyID];

		auto it = _keyMap.find(keyID);
		//если нашло, вернет
		if (it != _keyMap.end()){
			return it->second;
		}
		else{
			return false;
		}
	}

	bool InputManager::isKeyPressed(unsigned int keyID){
		//проверка была ли нажата клавиша на этом кадре, но не нажата на прошлом
		if (isKeyDown(keyID) ==true && wasKeyDown(keyID) == false){
			return true;
		}
		return false;
	}
	void InputManager::setMouseCoords(float x, float y){
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}

	bool InputManager::wasKeyDown(unsigned int keyID){
		auto it = _previousKeyMap.find(keyID);
		//если нашло, вернет
		if (it != _previousKeyMap.end()){
			return it->second;
		}
		else{
			return false;
		}
	}
}