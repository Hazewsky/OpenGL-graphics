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
	//программа шейдеров
	Bengine::GLSLProgram m_textureProgram;
	//камера
	Bengine::Camera2D m_camera;
	//спрайт батч, для отрисовки
	Bengine::SpriteBatch m_spriteBatch;
	//текстурка
	Bengine::GLTexture m_texture;
	//ИГрок
	Player m_player;
	//Окошечко
	Bengine::Window* m_window;
	//Создаем Физический 2Д мир, где все объекты, тела, грани и т.д.
	std::unique_ptr<b2World> m_world;
	//Вектор Ящиков, не юзаем указатели, ибо у них уже есть указатели к памяти что аллоцирована внутри мира
	std::vector<Box> m_boxes;
};

