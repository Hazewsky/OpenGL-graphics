#include "AudioEngine.h"
#include "BengineErrors.h"
namespace Bengine{

void SoundEffect::play(int loops /*= 0*/){
	//�� ����� ���� ����� ����� ����� ������ �����, � �� ������ 1
	//-1 - ������ ��������� �����
	//loops - ������� ��� ����� ������
	if (Mix_PlayChannel(-1, z_chunk, loops) == -1){
		//���� ������ ��� ��� ������, �� ������� ��������� �� 1 �����
		//���� � ����� �����, �� ��� ���� �����
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
//������ ����� ������������� SDL
void AudioEngine::init(){

	if (z_isInitialized){
		//return;
		fatalError("Tried to init AudioEngine twice");
	}
	//�������� ����� ���� ������� ��������� ���������� MIX_INIT_FAC
	//MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
	//����� � wav ����� �� �������
	if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1){
		//Mix_GetError ������ ������� ������
		fatalError("MIX_INIT error : " + std::string(Mix_GetError()));
	}
	//22050 - ������� �� ��������� * 2 ��� ������� ��������� ��3
	//������ �� �������
	//Channels - 1 ��� ���� , 2 ��� ������
	//���-�� ������ ������� ����� ��������������� ������������. ������� ������ ������ ����
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1){
		fatalError("MIX_OpenAudio error : " + std::string(Mix_GetError()));
	}
	z_isInitialized = true;
}
void AudioEngine::destroy(){
	if (z_isInitialized){
		z_isInitialized = false;
		//�� ����� ��������� � end � begin
		for (auto& it : z_effectMap){
			Mix_FreeChunk(it.second);
		}
		for (auto&it : z_musicMap){
			Mix_FreeMusic(it.second);
		}
		z_effectMap.clear();
		z_musicMap.clear();
		//��������� � OpenAudio
		Mix_CloseAudio();
		//�������, �� �� ����������� �������. ��������� Init
		Mix_Quit();
	}


}
//������ ����� �������� � 8 ����
//����� ��� ������
SoundEffect AudioEngine::loadSoundEffect(const std::string& filePath){
	//��������, ������� ����� ����� � ����
	auto it = z_effectMap.find(filePath);

	SoundEffect effect;
	
	if (it == z_effectMap.end()){
		//������ �� �����, ���� �������
		Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());

		
		//�������� �� ������
		if (chunk == nullptr){
			fatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()));
		}
		effect.z_chunk = chunk;
		z_effectMap[filePath] = chunk;
	}
	else{
		//��� � ����
		effect.z_chunk = it->second;
	}
	return effect;
}
Music AudioEngine::loadMusic(const std::string & filePath){
	//��������, ������� ����� ����� � ����
	auto it = z_musicMap.find(filePath);

	Music music;

	if (it == z_musicMap.end()){
		//������ �� �����, ���� �������
		Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());


		//�������� �� ������
		if (mixMusic == nullptr){
			fatalError("Mix_LoadMUS error: " + std::string(Mix_GetError()));
		}
		music.z_music = mixMusic;
		z_musicMap[filePath] = mixMusic;
	}
	else{
		//��� � ����
		music.z_music = it->second;
	}
	return music;

}


}