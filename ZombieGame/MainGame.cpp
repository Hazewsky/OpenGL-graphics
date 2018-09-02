#include "MainGame.h"

#include <Bengine/Bengine.h>
#include <SDL/SDL.h>
#include <iostream>
#include <Bengine/Timing.h>
#include <random>
#include <ctime>
#include <Bengine/BengineErrors.h>
#include <Bengine/ResourceManager.h>
#include <glm/gtx/rotate_vector.hpp>
#include "Zombie.h"
#include "Gun.h"
#include <algorithm>
const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;
const float DEG_TO_RAD = M_PI / 180.0f;
MainGame::MainGame() :
_screenWidth(900),
_screenHeight(600),
_gameState(GameState::PLAY),
_fps(0),
_player(nullptr),
_numHumansKilled(0),
_numZombiesKilled(0)
{
}


MainGame::~MainGame()
{
	//осовбождение памяти выделенной на уровни
	for (auto & lev : _levels){
		delete lev;
	}
	for (auto &hu : _humans){
		delete hu;
	}
	for (auto &zo : _zombies){
		delete zo;
	}
	delete[] _player;
	
}

void MainGame::run(){
	//первым делом инициализируем системы. загрузка уровня, и.т.д
	initSystems();
	//init level
	initLevel();

	//херачим музыку
	Bengine::Music music =  z_audioEngine.loadMusic("Sound/XYZ.ogg");
	music.play(-1);
	//и переходим к циклу отрисовки
	gameLoop();
	
}
/*
Это без лямбды. Эту функцию потом передаем в качестве параметра и се
void updateBloodParticle(Bengine::Particle2D& particle, float deltaTime){
	particle.p_position += particle.p_velocity * deltaTime;
	//эффект выцветания
	particle.p_color.a = (GLubyte(particle.p_life * 255.0f));
} */


	//инициализирует ядро программы
void MainGame::initSystems(){
	//инициализация движка
	Bengine::init();
	//инициализация звука( только после иниц. SDL!!)
	z_audioEngine.init();
	//создание окна
	_window.create("ZombieGame", _screenWidth, _screenHeight,0);
	//цвет фона.можно июзать текстуру как-то
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	//инициализация шейдеров
	initShaders();
	
	_agentSpriteBatch.init();///< спрайт агентов
	_hudSpriteBatch.init();///<спрайт шрифтов

	//инициализация шрифтов ТОЛЬКО после инициализовании SDL и шейдеров и OpenGL, иначе ошибки
	//путь к файлу и разрешение(также и размер, регулируется scale'ом при отрисовке)! Внимание, не рассчитан больше чем на 4092 по размеру
	//Но это можно исправить в классе. Главное, дабы была степень двойки
	//first printable char & last printable char указывает диапазон в ASCII(по умолчание 32,126)
	//cs,ce - свой диапазон, он не должен выходить за рамки указанного (32-126) я еще не разобрался как его изменить на кириллицу
	_spriteFont = new Bengine::SpriteFont("Fonts/horrendo.ttf", 64);
	//инициализация камеры с размером экрана
	_camera.init(_screenWidth,_screenHeight);
	_hudCamera.init(_screenWidth, _screenHeight);
	//_hudCamera.setPosition(glm::vec2(_screenWidth/2, _screenHeight/2));

	int a = 100;
	//инициализация частиц
	p_bloodParticleBatch = new Bengine::ParticleBatch2D;
	p_bloodParticleBatch->init(1000,
								0.05f,
								Bengine::ResourceManager::getTexture("Textures/particle.png"), 
								//updateBloodParticle
								//лямбда. ЧТобы не тратить место на создание функции, файла с функциями с ит.д.
								//Херачим функцию прямо тут
								//пустые квадратные скобки значат что мы не используем никаких внешних переменных
								//= это использование по значению
								// & по ссылке(в случае с а очень плохо, ибо она стековая переменная(будет not allocate with memory)
								[=](Bengine::Particle2D& particle, float deltaTime) {
		particle.p_position += particle.p_velocity * deltaTime;
		//эффект выцветания
		particle.p_color.a = (GLubyte(particle.p_life * 255.0f));
	});
	p_particleEngine.addParticleBatch(p_bloodParticleBatch);
}

void MainGame::initLevel(){
	//уровень 1
	_levels.push_back(new Level("Levels/level1.txt"));
	//указываем что начинаем с 1 уровня
	_currentLevel = 0;
	_player = new Player();
	_player->init(5.0f, _levels[_currentLevel]->getStartPlayerPos(), &_inputManager, &_camera, &_bullets);
	//т.к. player наследует humans, мы можем хранить его в векторе humans
	_humans.push_back(_player);

	//рандомим людишек
	//движок рандома, хороший тип
	static std::mt19937 randomEngine;
	//постоянный рандом
	randomEngine.seed(time(nullptr));
	//границы. только целочисленные
	//-2 ибо мы не хотим спавнить на границах
	//уровень должен быть квадратным, иначе будет не ок


	std::uniform_int_distribution<int> randX(2, _levels[_currentLevel]->getWidth()-2);
	std::uniform_int_distribution<int> randY(2, _levels[_currentLevel]->getHeight()-2);

	//добавить всех рандомных людишек
	for (int i = 0; i < _levels[_currentLevel]->getNumHumans(); i++){
		//_humans.emplace_back(randX(randomEngine, randY(randomEngine)));
		_humans.push_back(new Human);
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) *TILE_WIDTH);
		_humans.back()->init(HUMAN_SPEED, pos);
	}
	//добавить зомбей
	const std::vector<glm::vec2> & zombiePositions = _levels[_currentLevel]->getZombieStartPos();
	for (auto & it : zombiePositions){
		_zombies.push_back(new Zombie);
		_zombies.back()->init(HUMAN_SPEED, it);
	}

	//добавляем крутые пушки ГГ
	//имя, скорость стрельбы)каждые н кадров), кол-во снарядов, разброс, скорость, дамаг
	const float BULLET_SPEED = 20.0f;
	_player->addGun(new Gun("Magnum", 10, 1, 1.0f, BULLET_SPEED, 30.0f,
		z_audioEngine.loadSoundEffect("Sound/shots/pistol.wav")));
	_player->addGun(new Gun("Shotgun",30, 8, 20.0f, BULLET_SPEED, 200.0f,
		z_audioEngine.loadSoundEffect("Sound/shots/shotgun.wav")));
	_player->addGun(new Gun("MP-5", 5, 1, 5.0f, BULLET_SPEED, 20.0f,
		z_audioEngine.loadSoundEffect("Sound/shots/cg1.wav")));

}
	//инициализирует шейдеры
void MainGame::initShaders(){
	//компилируем шейдеры цвет
	_textureProgram.compileShaders("Shaders/VerticesShader.txt", "Shaders/FragmentShader.txt");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}
	//цикл программы. возможность играть, собственно. отрисовка там, движения и т.д
void MainGame::gameLoop(){
	Bengine::FpsLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(60.0f);
	//zoom out by 3 times
	const float CAMERA_SCALE = 1.0f / 3.0f;
	_camera.setScale(CAMERA_SCALE);

	//TIme step. Без него проблема заключается в том, что приложение собирается, допустим под 60 фпс, но может быть меньше, больше.
	//Если меньше, игра будет замедляться
	//больше - ускоряться
	//При 120 фпс игра будет в 2 раза быстрее
	//это тот фпс который нам нужен, на который залочена игра
	
	const float DESIRED_FPS = 60.0f;
	//максимальное кол-во кадров которые мы можем симулировать
	const int MAX_PHYSICS_STEPS = 6;
	const float MS_PER_SECOND = 1000;
	//FRAME_TIME сколько миллисекунд тратится на 1 кадр
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
	//максимальный разрешенный timestep. 1.0 значит без замедления и ускорения
	const float MAX_DELTA_TIME = 1.0f;
	//получаем текущий frametime
	//SDL_GETTICKS возвращает сколько прошло миллисекунд с тех пор как сдл был инициализирован
	float previousTicks = SDL_GetTicks();
	while (_gameState == GameState::PLAY){
		//ограничалка фпс
		fpsLimiter.begin();
		float newTicks = SDL_GetTicks();
		//сколько миллисекунд занял кадр
		float frameTime = newTicks - previousTicks;
		//обновление переменной
		previousTicks = newTicks;

		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;
		checkVictory();
		_inputManager.update();
		//обработчик ввода
		processInput();
		//Это должно выровнять игру, но есть одна проблемка.
		//если у нас будет дохера людей, поймаем spire loop death
		//мы пытаемся симулировать больше и больше кадров, но не переходим к отрисовке
		//потому что наша симуляция ТАКАЯ ДОЛГАЯ
		int i = 0;
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS){
			//если будем запускать на 120 фпс, totalDeltaTime будет меньше(0.5), и игра будет замедляться

			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			//обновляем агентов
			updateAgents(deltaTime);
			//обновляем пуля
			updateBullets(deltaTime);
			//обновление частиц
			p_particleEngine.update(totalDeltaTime);
			//тип все пофикшено, чуток избавляемся от totalDeltaTime
			//нахуя?
			totalDeltaTime -= deltaTime;
			i++;
		}

		
		//обновляем камеру
		_camera.setPosition(_player->getPosition());
		_camera.update();
		_hudCamera.update();
		drawGame();


		_fps=fpsLimiter.end();
		std::cout << _fps << std::endl;
	}
}

void MainGame::updateAgents(float deltaTime){
	//обновление всех людей
	for (auto & it : _humans){
		it->update(_levels[_currentLevel]->getLevelData(),
			_humans,
			_zombies, deltaTime);
	}
	//обновление зомбей
	for (auto & it : _zombies){
		it->update(_levels[_currentLevel]->getLevelData(),
			_humans,
			_zombies, deltaTime);
	}
	//обновление коллизий зомби
	for (int i = 0; i < _zombies.size(); i++){
		//с другими зомби
		for (int j = i + 1; j < _zombies.size(); j++){
			_zombies[i]->collideWithAgent(_zombies[j]);
		}
		//с людьми
		//j=0 игрок. сломает игру
		for (int j = 1; j < _humans.size(); j++){
			if (_zombies[i]->collideWithAgent(_humans[j])){
				_zombies.push_back(new Zombie);
				//плиим зомби в той же саомй позиции где был "зараженный" хум
				_zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());
				//удаляем хума
				delete _humans[j];
				_humans[j] = _humans.back();
				_humans.pop_back();

			}
		}
		//коллизия с игроком
		if (_zombies[i]->collideWithAgent(_player)){
			_gameState = GameState::EXIT;
			Bengine::fatalError("YOU, LOOSER!");
			
			}
	}
	//обновление коллизий людей
	//каждый человек проверяется на коллайд со всеми остальными
	/*for (auto& i : _humans){
		for (auto & j)
	}*/
	for (int i = 0; i < _humans.size(); i++){
		//коллизии людей с людьми
		//позволить проходить через меьншее кол-во людей. ибо вычитается сам хуман, который проверяется
		for (int j = i + 1; j < _humans.size(); j++){
			_humans[i]->collideWithAgent(_humans[j]);
		}
	}
	//Не забудь обновить зомбаков
}

void MainGame::updateBullets(float deltaTime){
	//обвновление и коллайд с миром
	for (int i = 0; i < _bullets.size();){
		//если update возвращает true, пуля соприкасается со стеной
		if (_bullets[i].update(_levels[_currentLevel]->getLevelData(), deltaTime)){
			//не надо делать delete [], ибо пуля то у нас не New
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		//если мы удалили пулю, нам не надо инкрементировать, чтобы корректно поменялись значения
		else{
			i++;
		}
	}

	bool wasBulletRemoved;
//коллайд с людьми и зомбями
	for (int i = 0; i < _bullets.size();i++){
		wasBulletRemoved = false;
		
		//луп через зомбей

		for (int j = 0; j < _zombies.size();){
			//проверка столкновения
			if (_bullets[i].collideWithAgent(_zombies[j])){
				//добавляем кровищи
				addBlood(_bullets[i].getPosition(), 7);

				//дамаг зомби и убить его, если хп == 0
				//ерем дамаг пули
				if (_zombies[j]->applyDamage(_bullets[i].getDamage())){
					//если зомби помер, удаляем его
					delete _zombies[j];
					_zombies[j] = _zombies.back();
					_zombies.pop_back();
					_numZombiesKilled++;
				}
				else{
					j++;
				}
				//удаляем пулю
				_bullets[i] = _bullets.back();
				_bullets.pop_back();

				//continue скипает все остальное
				//break прекратит итерация цикла зомби
				//Почему?
				//потому что пуля умерла, и продолжать цикл нет смысла.
				//вот был бы лазер, который пробивает насквозь, тогда да.ф
				//т.к. идет итерация всегда, а мы удалили пулю, надо вернуться назад
				//и убеждаемся, что не скипнули пулю.иначе у нас будет на одну пулю меньше, полагаю
				wasBulletRemoved = true;
				i--;
				break;
			}
			else{
				j++;
			}
		}
		//следующая пуля. Если мы дошли сюда, значит пуля не была удалена. промах
		//луп сквозь людей
		//если пуля не умерла
		if (wasBulletRemoved == false){
			for (int j = 1; j < _humans.size();){
				//проверка столкновения
				if (_bullets[i].collideWithAgent(_humans[j])){
					//ебашим кровищи
					addBlood(_bullets[i].getPosition(), 5);

					//дамаг зомби и убить его, если хп == 0
					//ерем дамаг пули
					if (_humans[j]->applyDamage(_bullets[i].getDamage())){
						//если зомби помер, удаляем его
						delete _humans[j];
						_humans[j] = _humans.back();
						_humans.pop_back();
						_numHumansKilled++;
					}
					else{
						j++;
					}
					//удаляем пулю
					_bullets[i] = _bullets.back();
					_bullets.pop_back();

					//continue скипает все остальное
					//break прекратит итерация цикла зомби
					//Почему?
					//потому что пуля умерла, и продолжать цикл нет смысла.
					//вот был бы лазер, который пробивает насквозь, тогда да.ф
					//т.к. идет итерация всегда, а мы удалили пулю, надо вернуться назад
					//и убеждаемся, что не скипнули пулю.иначе у нас будет на одну пулю меньше, полагаю
					wasBulletRemoved = true;
					i--;
					break;
				}
				else{
					j++;
				}
			}
		}
		
	}
}
//Условия победы
void MainGame::checkVictory(){
	//TODO: Поддержка нескольких уровней
	//_currentLevel++;
	//initLevel(...);
	//если все зомби мертвы, мы победуны
	if (_zombies.empty()){
		std::printf("****You win!****\n You killed %d humans and %d zombies. There are %d/%d humans remaining",
			_numHumansKilled, _numZombiesKilled, _humans.size() - 1, _levels[_currentLevel]->getNumHumans());
		Bengine::fatalError("");
	}
}
	//обработчик нажатий
void MainGame::processInput(){
	SDL_Event evnt;
	//будет крутиться пока есть процессы для обработки
	while (SDL_PollEvent(&evnt)){
		switch (evnt.type){
		case SDL_QUIT:
			//Выход здесь
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}
}

	//рендер игры
void MainGame::drawGame(){
	//Пилим базовую глубину 1.0
	glClearDepth(1.0f);
	//Очищаем буфера цвета и глубины
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_textureProgram.use();
	//Здесь код отрисовки
	//Говорим что хотим использовать Texture unit 0
	glActiveTexture(GL_TEXTURE0);
	//проверяем что mySampler тоже юзает этот юнит
	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	//получаем матрицу камеры
	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	//получаем uniform из камеры
	GLint pUniform = _textureProgram.getUniformLocation("P");
	//&projectionMatrix[0][0] самое его начало
	glUniformMatrix4fv(pUniform, 1,GL_FALSE, &projectionMatrix[0][0]);


	//рисуем уровень
	_levels[_currentLevel]->draw();
	//начало рисования агентов
	_agentSpriteBatch.begin();

	//убрать const если будут разные размеры агентов
	const glm::vec2 agentDims(AGENT_RADIUS * 2.0f);
//	рисуем людей

	for (auto & it : _humans){
		if (_camera.isBoxInView(it->getPosition(), agentDims)){
			it->draw(_agentSpriteBatch);
		}
	}
	//	рисуем зомбей
	for (auto & it : _zombies){
		if (_camera.isBoxInView(it->getPosition(), agentDims)){
			it->draw(_agentSpriteBatch);
		}
	}
	//рисуем пули
	for (auto &it : _bullets){
		it.draw(_agentSpriteBatch);
	}

	_agentSpriteBatch.end();
	_agentSpriteBatch.renderBatch();
	//отрисовка частиц
	p_particleEngine.draw(&_agentSpriteBatch);
	//отрисовка сообщений
	drawHud();
	_textureProgram.unuse();
	//Меняем буффер(у нас их 2) и отрисовываем все на экран
	_window.swapBuffer();
}

void MainGame::drawHud(){
	//буфер символов больше чем нужен
	char buffer[256];

	glm::mat4 projectionMatrix = _hudCamera.getCameraMatrix();
	//получаем uniform из камеры
	GLint pUniform = _textureProgram.getUniformLocation("P");
	//&projectionMatrix[0][0] самое его начало
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	_hudSpriteBatch.begin();
	//вместо печатели на экран, печатает в buffer
	sprintf_s(buffer, "Humber of Humans alive %d", _humans.size());
	//если buffer был бы string, то надо было юзать еще и c_str()
	//justification -выравнивание. Left дефолт
	_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(50, -200), glm::vec2(0.5f), 0.0f,
		Bengine::Color(0, 0, 0, 255), Bengine::Justification::MIDDLE);
	sprintf_s(buffer, "Humber of Zombies alive %d", _zombies.size());
	_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(50, -232), glm::vec2(0.5f), 0.0f,
		Bengine::Color(0, 0, 0, 255), Bengine::Justification::MIDDLE);
	_hudSpriteBatch.end();
	_hudSpriteBatch.renderBatch();
}

void MainGame::addBlood(const glm::vec2& position, int numParticles){
	//движок рандома, хороший тип
	static std::mt19937 randomEngine(time(nullptr));
//гарницы
	static std::uniform_real_distribution<float> randAngle(0.0f * DEG_TO_RAD, 360.0f * DEG_TO_RAD);

	glm::vec2 vel(2.0f, 0.0f);
	for (int i = 0; i < numParticles; i++){
		p_bloodParticleBatch->addParticle(position,
											glm::rotate(vel, randAngle(randomEngine)),
											Bengine::Color(255, 0, 0, 255),
											20.0f);
	}
}