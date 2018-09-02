#pragma once
#include <SDL/SDL_mixer.h>
#include <string>
#include <map>
namespace Bengine{

class SoundEffect{
public:
	//����� ����� AudioEngine ��� ������� ��� ������ ��������.
	friend class AudioEngine;
	//-1 loops ������ ���������, ����� ������ ��������� ���������� ��� +1
	void play(int loops = 0);
private:
	//��� ��������
	Mix_Chunk* z_chunk = nullptr;

};

class Music{
public:
	friend class AudioEngine;
	//-1 loops ������ ���������, ����� ������ ��������� ���������� ���
	void play(int loops= 1);
	//��� ������ ����� �� 1 ����, ������ ������. �� ��� �� �����������, ������ ���, �� �����, �������� ��� ������
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
	//������ ����� �������� � 8 ����
	//����� ��� ������
	SoundEffect loadSoundEffect(const std::string& filePath);
	Music loadMusic(const std::string & filePath);
private:
	//��� ��� ����� ������
	//���� � ����� � ���� ��������
	//��� ���� �������.
	//�� ����� ����������� ��������� �������� ������������.
	//� ��� ������ - ���
	std::map<std::string, Mix_Chunk*> z_effectMap;
	std::map<std::string, Mix_Music*> z_musicMap;

	bool z_isInitialized = false;
};



}

