#pragma once



namespace Bengine{

class FpsLimiter{
public:
	FpsLimiter();
	void init(float maxFPS);

	void setMaxFPS(float maxFPS);

	void begin();

	//вернет текущий FPS
	float end();
	
private:
	void calculateFPS();
	//количество кадров в секунду
	float _fps;
	float _frameTime;
	
	float _maxFPS;
	unsigned int _startTicks;
};
}