#include "MainGame.h"
#include <iostream>
#include <glm/glm.hpp>
//#include <Windows.h>
//#include <SDL/SDL.h>
//#include <GL/glew.h>
//** arrayof strings
int main(int argc, char** argv){//command line arguments
	//prjoect/properties VC++ Directories include + library
	//project/properties/linker/system->console && linker/input->additional depencencies( all .lib files)
	//then put the sdl and glew .dll file in debug folder
	SDL_Init(SDL_INIT_EVERYTHING);
	MainGame mainGame;
	mainGame.run();

	return 0;
}