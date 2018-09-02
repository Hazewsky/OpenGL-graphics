#define _CRT_SECURE_NO_WARNINGS // To shut up the compiler about sprintf...
#include "MainGame.h"

#include <Bengine/Bengine.h>
#include <Bengine/ResourceManager.h>
#include <SDL/SDL.h>
#include <random>
#include <ctime>
#include <algorithm>
#include <cmath>

// Полезные константы
const float DESIRED_FPS = 60.0f; // ФПС при котором будет запускаться игра
const int MAX_PHYSICS_STEPS = 6; // Максимальное количество физических шагов на кадр
const float MS_PER_SECOND = 1000; // КОл-во мс в секунде
const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS; // Желаемое время кадра на 1 кадр
const float MAX_DELTA_TIME = 1.0f; // Максимум дельты

void MainGame::run(){
	init();
	initBalls();

	//старт переменной показывающей Кол-во тиков с момента запуска
	Uint32 previousTicks = SDL_GetTicks();

	//Луп игры
	while (gameState_ == GameState::RUNNING){
		fpsLimiter_.begin();
		processInput();

		//Вычисление времени кадра в мс
		Uint32 newTicks = SDL_GetTicks();
		Uint32 frameTime = newTicks - previousTicks;
		previousTicks = newTicks;//храним NewTicks в Prev чтобы мы могли использовать его в след. кадре

		// Получение общей дельты
		float totalDeltaTime = (float)frameTime / DESIRED_FRAMETIME;

		int i = 0; // Этот счетчик не даст нам закрутиться насмерть
		// Луп когда у нас еще есть физ. шаги которые надо сделать
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
			//deltaTime должно быть меньшим между totalDeltaTime and MAX_DELTA_TIME;
 
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			// Update all physics here and pass in deltaTime

			update(deltaTime);

			// Since we just took a step that is length deltaTime, subtract from totalDeltaTime
			totalDeltaTime -= deltaTime;
			// Increment our frame counter so we can limit steps to MAX_PHYSICS_STEPS
			i++;
		}
		camera_.update();
		draw();
		fps_ = fpsLimiter_.end();
	}
}

void MainGame::init(){
	Bengine::init();

	screenWidth_ = 500;
	screenHeight_ = 500;
	window_.create("Ball Game", screenWidth_, screenHeight_, 0);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	camera_.init(screenWidth_, screenHeight_);
	//Направляем камеру в центр экрана
	camera_.setPosition(glm::vec2(screenWidth_ / 2.0f, screenHeight_ / 2.0f));

	spriteBatch_.init();
	//инициализация шрифта
	spriteFont_ = std::make_unique<Bengine::SpriteFont>("Fonts/chintzy.ttf", 40);
	//компилируем текстурный шейдер
	textureProgram_.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	textureProgram_.addAttribute("vertexPosition");
	textureProgram_.addAttribute("vertexColor");
	textureProgram_.addAttribute("vertexUV");
	textureProgram_.linkShaders();

	fpsLimiter_.setMaxFPS(60.0f);
}

struct BallSpawn{
	BallSpawn(const Bengine::Color& colr,
			float rad, float m, float minSpeed,
			float maxSpeed, float prob):
			color(colr),
			radius(rad),
			mass(m),
			randSpeed(minSpeed, maxSpeed),
			probability(prob)
			{
	//empty
	}
	Bengine::Color color;
	float radius;
	float mass;
	float probability;
	std::uniform_real_distribution<float> randSpeed;
};
#include <iostream>
void MainGame::initBalls(){

#define ADD_BALL(p,...)\
	totalProbability += p; \
	possibleBalls.emplace_back(__VA_ARGS__);

	//КОличество шаров для спавна
	const int NUM_BALLS = 100;

	//Рандомизация
	std::mt19937 randomEngine((unsigned int)time(nullptr));
	std::uniform_real_distribution<float> randX(0.0f, (float)screenWidth_);
	std::uniform_real_distribution<float> randY(0.0f, (float)screenHeight_);
	std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

//Добавление всех возможных шаров
	std::vector<BallSpawn> possibleBalls;
//вероятность
	float totalProbability = 0.0f;

//Добавление шаров используя макро
	ADD_BALL(20.0f, Bengine::Color(255, 255, 255, 255),
		20.0f, 1.0f, 0.1f, 7.0f, totalProbability);
	ADD_BALL(10.0f, Bengine::Color(0, 0, 255, 255),
		30.0f, 2.0f, 0.1f, 3.0f, totalProbability);
	ADD_BALL(1.0f, Bengine::Color(255, 0, 0, 255),
		50.0f, 4.0f, 0.0f, 0.0f, totalProbability);

//случайная вероятность для спавна шаров
	std::uniform_real_distribution<float> spawn(0.0f, totalProbability);
//маленькая оптимизация
//Устанавливает размер внутреннего массива дабы
//Предотвратить доп. аллокацию(распределение)
	balls_.reserve(NUM_BALLS);

//спавн шара с дефолтными значениями
	BallSpawn* ballToSpawn = &possibleBalls[0];
	for (int i = 0; i < NUM_BALLS; i++){
		//спавн ролл
		float spawnVal = spawn(randomEngine);
		//выясняем какой шар берем
		for (auto& it : possibleBalls){
			if (spawnVal <= it.probability){
				ballToSpawn = &it;
				break;
			}
		}
		//Случайные стартовые позиции
		glm::vec2 pos(randX(randomEngine), randY(randomEngine));
		//Хитрожопый путь чтобы получить случайное направление
		glm::vec2 direction(randDir(randomEngine), randDir(randomEngine));
		if (direction.x != 0.0f || direction.y != 0.0f){ //Шансы что направление == 0 астрономически малы
			direction = glm::normalize(direction);
		}
		else{
			direction = glm::vec2(1.0f, 0.0f);//значение по умолчанию
		}
		//Добавление шара
		balls_.emplace_back(ballToSpawn->radius,
			ballToSpawn->mass, pos,
			direction * ballToSpawn->randSpeed(randomEngine),
			Bengine::ResourceManager::getTexture("Textures/circle.png").id,
			ballToSpawn->color);

	}
}

void MainGame::update(float deltaTime){
	ballController_.updateBalls(balls_, deltaTime, screenWidth_, screenHeight_);
}

void MainGame::draw(){
	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	textureProgram_.use();

	glActiveTexture(GL_TEXTURE0);

	// Make sure the shader uses texture 0
	GLint textureUniform = textureProgram_.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	// Grab the camera matrix
	glm::mat4 projectionMatrix = camera_.getCameraMatrix();
	GLint pUniform = textureProgram_.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	spriteBatch_.begin();

	// Draw balls
	for (auto& ball : balls_) {
		ballRenderer_.renderBall(spriteBatch_, ball);
	}

	spriteBatch_.end();
	spriteBatch_.renderBatch();

	drawHud();

	textureProgram_.unuse();

	window_.swapBuffer();
}

void MainGame::drawHud(){
	const Bengine::Color fontColor(255, 0, 0, 255);
	//конвертируем флоат в массив чаров
	char buffer[64];
	//запись в строку
	sprintf(buffer, "%.1f", fps_);
	spriteBatch_.begin();
	spriteFont_->draw(spriteBatch_, buffer,
		glm::vec2(0.0f, screenHeight_ - 32.0f),
		glm::vec2(1.0f), 0.0f, fontColor);
	spriteBatch_.end();
	spriteBatch_.renderBatch();
}

void MainGame::processInput(){
	SDL_Event evnt;
	//Будет крутиться пока будет что-то не останется евентов
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			gameState_ = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			ballController_.onMouseMove(balls_, (float)evnt.motion.x, (float)screenHeight_ - (float)evnt.motion.y);
			inputManager_.setMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			inputManager_.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			inputManager_.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			ballController_.onMouseDown(balls_, (float)evnt.button.x, (float)screenHeight_ - (float)evnt.button.y);
			inputManager_.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			ballController_.onMouseUp(balls_);
			inputManager_.releaseKey(evnt.button.button);
			break;
		}
	}

	if (inputManager_.isKeyPressed(SDLK_ESCAPE)) {
		gameState_ = GameState::EXIT;
	}
	//МЕНЯЕМ ГРАВИТАЦИЮ УИИ
	if (inputManager_.isKeyPressed(SDLK_LEFT)) {
		ballController_.setGravityDirecton(GravityDirection::LEFT);
	}
	else if (inputManager_.isKeyPressed(SDLK_RIGHT)) {
		ballController_.setGravityDirecton(GravityDirection::RIGHT);
		
	}
	else if (inputManager_.isKeyPressed(SDLK_UP)) {
		ballController_.setGravityDirecton(GravityDirection::UP);
	}
	else if (inputManager_.isKeyPressed(SDLK_DOWN)) {
		ballController_.setGravityDirecton(GravityDirection::DOWN);
	}
	else if (inputManager_.isKeyPressed(SDLK_SPACE)) {
		ballController_.setGravityDirecton(GravityDirection::NONE);
	}
}