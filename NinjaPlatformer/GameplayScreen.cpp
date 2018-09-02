#include "GameplayScreen.h"
#include <iostream>
#include <SDL/SDL.h>
#include <Bengine/IMainGame.h>
#include <Bengine/ResourceManager.h>
#include <random>

GameplayScreen::GameplayScreen(Bengine::Window* window) : m_window(window) {
}

GameplayScreen::~GameplayScreen() {

}

int GameplayScreen::getNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int GameplayScreen::getPreviousScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

void GameplayScreen::build() {
}

void GameplayScreen::destroy() {
}

void GameplayScreen::onEntry() {
//подключаем мир
//гравитация, x y
	b2Vec2 gravity(0.0f,-15.0f);
	m_world = std::make_unique<b2World>(gravity);
//создаем физ. объекты (bodies) solid box
	//shape - 2D геометрический объект
//fixture - биндит shape в body, может быть несколько

	//пилим поверхность
	//создаем тело. У каждого физ. объекта должно быть тело
	//дефиниция
	b2BodyDef groundBodyDef;
	//позиция. Все юниты в МЕТРАХ
	groundBodyDef.position.Set(0.0f, -15.0f);
	//Пили сосбвтенно тело. , аллоцируем тело в мире используя деф.
	//Деф можем исопльзоваться чтобы делать разные копии тел
	//Деф - лист параметров, используемых когда создаем тело
	b2Body* groundBody = m_world->CreateBody(&groundBodyDef);
	//Ground fixture
	b2PolygonShape groundBox;
	//создает в форме коробки
	//в скобках - только половина. При 50 на самом деле будет 100
	groundBox.SetAsBox(50.0f, 10.0f);
	//создаем fixture, то была тоже дифиниция? аллоцированная в памяти
	//2- dinsity, т.к. не будет двигаться, будет статична
	groundBody->CreateFixture(&groundBox, 0.0f);

	//Грузим текстуру
	m_texture = Bengine::ResourceManager::getTexture("Assets/glass.png");


	//Пилим КУЧУ коробок
	std::mt19937 randGenerator;
	std::uniform_real_distribution<float> xPos(-10.0f, 10.0f);
	std::uniform_real_distribution<float> yPos(-10.0f, 15.0f);
	std::uniform_real_distribution<float> size(0.5f, 2.5f);
	std::uniform_int_distribution<int> color(0, 255);
	const int NUM_BOXES = 50;
	
	

	for (int i = 0; i < NUM_BOXES; i++){
		Bengine::Color randColor;
		randColor.r = color(randGenerator);
		randColor.g = color(randGenerator);
		randColor.b = color(randGenerator);
		randColor.a = 255;
		Box newBox;
		newBox.init(m_world.get(), glm::vec2(xPos(randGenerator), yPos(randGenerator)), glm::vec2(size(randGenerator), size(randGenerator)), m_texture, randColor, false);
		//После этого newBox уже не нужно, ибо все уже аллоцировано внутри m_world
		m_boxes.push_back(newBox);
		//а можно и emplace_back
	}
	//Инициализируем SpriteBatch
	m_spriteBatch.init();

	//Шейдеры
	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();

	

	//Подрубаем камеру
	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	//32 пикселя на метр
	m_camera.setScale(32.0f);

	//инициализация игрока
	m_player.init(m_world.get(), glm::vec2(0.0f,30.0f), glm::vec2(2.0f, 2.0f), Bengine::Color(255,255,255,255));

}

void GameplayScreen::onExit() {
}

void GameplayScreen::update() {
	m_camera.update();

	checkInput();
	//апдейт игрока
	m_player.update(m_game->inputManager);
	//Апдейт физики
	//60 fps
	//6, 2 документация
	m_world->Step(1.0f / 60.0f, 6, 2);
	
}

void GameplayScreen::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	m_textureProgram.use();

	//Грузим texture uniform
	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	//Матрица Камеры
	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);
	m_spriteBatch.begin();
	//рисуем все коробки
	for (auto& b : m_boxes){
		b.draw(m_spriteBatch);
	}
	//рисуем игрока
	m_player.draw(m_spriteBatch);
	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
	m_textureProgram.unuse();
}

void GameplayScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->onSDLEvent(evnt);
	}
}
