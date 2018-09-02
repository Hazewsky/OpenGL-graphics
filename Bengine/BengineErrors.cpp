#include "BengineErrors.h"
#include <iostream>
#include <SDL/SDL.h>
namespace Bengine{
	void fatalError(std::string errorString){
		std::cout << errorString << std::endl;
		std::cout << "Enter any key to quit...";
		int tmp;
		std::cin >> tmp;
		SDL_Quit(); //выключает SDL
		exit(1);
	}
}