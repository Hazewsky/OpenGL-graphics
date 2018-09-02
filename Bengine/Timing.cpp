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

	//������ ������� FPS
	float FpsLimiter::end(){
		calculateFPS();

		float frameTicks = SDL_GetTicks() - _startTicks;
		//limit hte fps to the max FPS
		//���������� delay. ��� ������ �������� game loop'a ���� sleep'a
		if (1000.0f / _maxFPS > frameTicks){
			//�� ������ ��������. �� ��������� ���������� ������ ��������� �� <16 ms
			SDL_Delay(1000.f / _maxFPS - frameTicks);
		}
		return _fps;
	}

	void FpsLimiter::calculateFPS(){
		//������, ��� �������������� ����� ������ �����. ��� global, �� �������� ������ ����� ������ � ���� �������
		//NUM_SAMPLES -> ������� ������ �� ����� ����� ��� �������� �������� ��������
		static const int NUM_SAMPLES = 10;
		//�����, ��� �������� �����. ��� const � int ����� ������ ������� �� ������. ����������, ������� ������ ������ ������������.
		static float frameTimes[NUM_SAMPLES];
		//��������� �� ������� ����. 0 � ������
		static int currentFrame = 0;
		//���������� ��� ������������ ������ �����
		static float prevTicks = SDL_GetTicks();
		//�� ������ ������
		float currentTicks;
		currentTicks = SDL_GetTicks();
		_frameTime = currentTicks - prevTicks;

		//������ �������� ������. ���� ��� = 3, �� ����� 0, 1, 2 � ��� ���������� 3 - ����� �������� � ����, �.�. 3%3 = 0
		frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

		//����������, ����� ����� ��������� ��� ����� ���������� � ������ ������
		prevTicks = currentTicks;
		//���� ���������� ������, ������� �� ����� ����������
		int count;
		//������ ����� ������, ����� �� ���� ������� �� 0
		currentFrame++;
		//���� � ��� ���� �� ���, ��� ������� ������� - ���������� ������ ���
		if (currentFrame < NUM_SAMPLES){
			count = currentFrame;
		}
		//���� � ��� ���� ���, ���������� ���
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