#pragma once
#include <Box2D/Box2D.h>
#include <Bengine/IGameScreen.h>
#include "Box.h"
#include "Player.h"
#include <vector>
#include <Bengine/SpriteBatch.h>
#include <Bengine/GLSLProgram.h>
#include <Bengine/Camera2D.h>
#include <Bengine/GLTexture.h>
#include <Bengine/Window.h>

// Our custom gameplay screen that inherits from the IGameScreen
class GameplayScreen : public Bengine::IGameScreen {
public:
	GameplayScreen(Bengine::Window* window);
	~GameplayScreen();

	virtual int getNextScreenIndex() const override;

	virtual int getPreviousScreenIndex() const override;

	virtual void build() override;

	virtual void destroy() override;

	virtual void onEntry() override;

	virtual void onExit() override;

	virtual void update() override;

	virtual void draw() override;

private:
	void checkInput();
	//��������� ��������
	Bengine::GLSLProgram m_textureProgram;
	//������
	Bengine::Camera2D m_camera;
	//������ ����, ��� ���������
	Bengine::SpriteBatch m_spriteBatch;
	//���������
	Bengine::GLTexture m_texture;
	//�����
	Player m_player;
	//��������
	Bengine::Window* m_window;
	//������� ���������� 2� ���, ��� ��� �������, ����, ����� � �.�.
	std::unique_ptr<b2World> m_world;
	//������ ������, �� ����� ���������, ��� � ��� ��� ���� ��������� � ������ ��� ������������ ������ ����
	std::vector<Box> m_boxes;
};

