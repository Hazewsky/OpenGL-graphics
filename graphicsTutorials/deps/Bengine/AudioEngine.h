#pragma once
#include <SDL/SDL_mixer.h>
#include <string>
#include <map>
namespace Bengine{

class SoundEffect{
public:
	//пилим чтобы AudioEngine как главный мог менять значения.
	friend class AudioEngine;
	//-1 loops играет постоянно, иначе играет указанное количество раз +1
	void play(int loops = 0);
private:
	//для эффектов
	Mix_Chunk* z_chunk = nullptr;

};

class Music{
public:
	friend class AudioEngine;
	//-1 loops играет постоянно, иначе играет указанное количество раз
	void play(int loops= 1);
	//они влияют всего на 1 вещь, потому статик. Но это не обязательно, просто они, де факто, работают как статик
	static void pause();
	static void stop();
	static void resume();
private:
	Mix_Music* z_music = nullptr;
};
class AudioEngine
{
public:
	AudioEngine();
	~AudioEngine();

	void init();
	void destroy();
	//вернет копию размером в 8 байт
	//будет кеш звуков
	SoundEffect loadSoundEffect(const std::string& filePath);
	Music loadMusic(const std::string & filePath);
private:
	//кеш для аудио файлов
	//путь к файлу и файл эффектов
	//тут есть разница.
	//мы можем проигрывать несколько эффектов одновременно.
	//а вот музыку - хуй
	std::map<std::string, Mix_Chunk*> z_effectMap;
	std::map<std::string, Mix_Music*> z_musicMap;

	bool z_isInitialized = false;
};



}

