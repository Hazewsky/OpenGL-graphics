#include "AudioEngine.h"
#include "BengineErrors.h"
namespace Bengine{

void SoundEffect::play(int loops /*= 0*/){
	//на самом деле лучше юзать самый старый канал, а не всегда 1
	//-1 - первый доступный канал
	//loops - сколько раз будет играть
	if (Mix_PlayChannel(-1, z_chunk, loops) == -1){
		//если ошибка ибо нет канала, то вручную перепишет на 1 канал
		//если и этому пизда, то нам всем пизда
		if (Mix_PlayChannel(0, z_chunk, loops) == -1){
			fatalError("MIX_PlayChannel error : " + std::string(Mix_GetError()));
		}
	}
}

void Music::play(int loops /*= -1*/){
	if (Mix_PlayMusic(z_music, loops) == -1){
		fatalError("MIX_PlayMusic error : " + std::string(Mix_GetError()));
	}
}


void Music::pause(){
	Mix_PauseMusic();
}
void Music::stop(){
	Mix_HaltMusic();

}
void Music::resume(){
	Mix_ResumeMusic();

}
AudioEngine::AudioEngine()
{
	//empty
}


AudioEngine::~AudioEngine()
{
	
	destroy();
}
//только после инициализации SDL
void AudioEngine::init(){

	if (z_isInitialized){
		//return;
		fatalError("Tried to init AudioEngine twice");
	}
	//параметр может быть битовой операцией комбинаций MIX_INIT_FAC
	//MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
	//также и wav файлы по дефолту
	if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1){
		//Mix_GetError вернет крайнюю ошибку
		fatalError("MIX_INIT error : " + std::string(Mix_GetError()));
	}
	//22050 - частота по умолчанию * 2 для лучшего воспроизв мп3
	//Формат по деволту
	//Channels - 1 для моно , 2 для стерео
	//кол-во байтов которое будет воспроизводится одновременно. Степень двойки должна быть
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1){
		fatalError("MIX_OpenAudio error : " + std::string(Mix_GetError()));
	}
	z_isInitialized = true;
}
void AudioEngine::destroy(){
	if (z_isInitialized){
		z_isInitialized = false;
		//на любую коллекцию с end и begin
		for (auto& it : z_effectMap){
			Mix_FreeChunk(it.second);
		}
		for (auto&it : z_musicMap){
			Mix_FreeMusic(it.second);
		}
		z_effectMap.clear();
		z_musicMap.clear();
		//обраточка к OpenAudio
		Mix_CloseAudio();
		//очистка, но не освобождает ресурсы. Обраточка Init
		Mix_Quit();
	}


}
//вернет копию размером в 8 байт
//будет кеш звуков
SoundEffect AudioEngine::loadSoundEffect(const std::string& filePath){
	//проверка, попытка найти аудио в кеше
	auto it = z_effectMap.find(filePath);

	SoundEffect effect;
	
	if (it == z_effectMap.end()){
		//ничего не нашло, надо грузить
		Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());

		
		//проверка на ошибки
		if (chunk == nullptr){
			fatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()));
		}
		effect.z_chunk = chunk;
		z_effectMap[filePath] = chunk;
	}
	else{
		//уже в кеше
		effect.z_chunk = it->second;
	}
	return effect;
}
Music AudioEngine::loadMusic(const std::string & filePath){
	//проверка, попытка найти аудио в кеше
	auto it = z_musicMap.find(filePath);

	Music music;

	if (it == z_musicMap.end()){
		//ничего не нашло, надо грузить
		Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());


		//проверка на ошибки
		if (mixMusic == nullptr){
			fatalError("Mix_LoadMUS error: " + std::string(Mix_GetError()));
		}
		music.z_music = mixMusic;
		z_musicMap[filePath] = mixMusic;
	}
	else{
		//уже в кеше
		music.z_music = it->second;
	}
	return music;

}


}