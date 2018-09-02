#include <SDL/SDL.h>
#include <GL/glew.h>
#include "Bengine.h"
namespace Bengine{
int init(){
	//initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);//говорит что нам нужен двойной буффер. Ѕудет два окна: на одном будем рисовать, когда другое очиститс€.—глаживание, хули
	return 0;
}

}