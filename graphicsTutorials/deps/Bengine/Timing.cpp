#include "Timing.h"

#include <SDL/SDL.h>

namespace Bengine{

	FpsLimiter::FpsLimiter(){

	}

	void FpsLimiter::init(float maxFPS){
		setMaxFPS(maxFPS);
	}

	void FpsLimiter::setMaxFPS(float maxFPS){
		_maxFPS = maxFPS;
	}

	void FpsLimiter::begin(){
		_startTicks = SDL_GetTicks();
	}

	//вернет текущий FPS
	float FpsLimiter::end(){
		calculateFPS();

		float frameTicks = SDL_GetTicks() - _startTicks;
		//limit hte fps to the max FPS
		//используем delay. Оно делает отсрочку game loop'a Типа sleep'a
		if (1000.0f / _maxFPS > frameTicks){
			//не всегда работает. На некоторых платформах нельзя острочить на <16 ms
			SDL_Delay(1000.f / _maxFPS - frameTicks);
		}
		return _fps;
	}

	void FpsLimiter::calculateFPS(){
		//статик, ибо использоваться будет только здесь. Как global, но получить доступ можно только в этой функции
		//NUM_SAMPLES -> сколько кадров мы будем брать для подсчета среднего значения
		static const int NUM_SAMPLES = 10;
		//буфер, где хранятся кадры. Без const в int такая запись вызвала бы ошибку. Показывает, сколько вообще кадров отрисовалось.
		static float frameTimes[NUM_SAMPLES];
		//указатель на текущий кадр. 0 в начале
		static int currentFrame = 0;
		//количество уже отображенных кадров ранее
		static float prevTicks = SDL_GetTicks();
		//на данный момент
		float currentTicks;
		currentTicks = SDL_GetTicks();
		_frameTime = currentTicks - prevTicks;

		//делаем круговой буффер. Если нум = 3, то будет 0, 1, 2 и при достижение 3 - снова вернется к нулю, т.к. 3%3 = 0
		frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

		//обновление, иначе будет считаться все время количество с самого начала
		prevTicks = currentTicks;
		//Тоже количество кадров, которые мы хотим подсчитать
		int count;
		//ставим перед блоком, чтобы не было деления на 0
		currentFrame++;
		//если у нас есть не все, что требует выборка - подсчитает только эти
		if (currentFrame < NUM_SAMPLES){
			count = currentFrame;
		}
		//если у нас есть все, подсчитает все
		else{
			count = NUM_SAMPLES;
		}

		float frameTimeAverage = 0;
		for (int i = 0; i < count; i++){
			frameTimeAverage += frameTimes[i];
		}
		frameTimeAverage /= count;

		if (frameTimeAverage > 0){
			_fps = 1000.0f / frameTimeAverage;
		}
		else{
			_fps = 60.0f;
		}
	}
}