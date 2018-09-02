#include "MainGame.h"
#include <Bengine/BengineErrors.h>
#include <iostream>
#include <string>
#include <Bengine/Bengine.h>
#include <Bengine/ResourceManager.h>


MainGame::MainGame() :
	_screenWidth(1000),
	_screenHeight(500),

	_time(0),
	_gameState(GameState::PLAY),
	_maxFPS(60.0f)
{
	_camera.init(_screenWidth, _screenHeight);
	//_window = nullptr; //защита от ошибки no access violation

}


MainGame::~MainGame()
{
}

void MainGame::run(){
	initSystems();
	/*_sprites.push_back( new Bengine::Sprite()); 
	_sprites.back()->init(0.0f, 0.0f, _screenWidth/2, _screenWidth/2, "Textures/1.png");

	_sprites.push_back(new Bengine::Sprite());
	_sprites.back()->init(_screenWidth/2, 0.0f, _screenWidth / 2, _screenWidth / 2, "Textures/1.png");*/

	/*_sprites.push_back(new Bengine::Sprite());
	_sprites.back()->init(-1.0f, 0.0f, 1.0f, 1.0f, "Textures/1.png");

	_sprites.push_back(new Bengine::Sprite());
	_sprites.back()->init(1.0f, 0.0f, -1.0f, 1.0f, "Textures/1.png");*/

	//_playerTexture = ImageLoader::loadPNG("Textures/1.png");
	gameLoop();
}

void MainGame::initSystems(){

	Bengine::init();

	_window.create("GameEngine", _screenWidth, _screenHeight, 0);

	initShaders();
	_spriteBatch.init();

	_fpsLimiter.init(_maxFPS);
}

void MainGame::initShaders(){
	_colorProgram.compileShaders("Shaders/colorShading.vert.txt", "Shaders/colorShading.frag.txt");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

void MainGame::gameLoop(){
	while (_gameState != GameState::EXIT){
		//замеряем сколько времени идет проход через весь этот блок
		_fpsLimiter.begin();
		processInput();
		//если надо крутой анимационный эффект
		_time += 0.1;
		//Обновить камеру
		_camera.update();
		//Обновить все пули
		for (int i = 0; i < _bullets.size();){
			//если ее _lifeTime == 0
			if (_bullets[i].update() ==true){
				//ставим пулю крайним элементом
				_bullets[i] = _bullets.back();
				//и удаляем нах. УДобно, что не надо париться насчет порядка
				_bullets.pop_back();
			}
			else{
				i++;
			}
			//мы инкерементируем только если не удалили пулю. Просто иначе _bullets.back() никогда не обновится
		}
		drawGame();
		//print only once every ten frames
		static int frameCounter = 0;
		frameCounter++;
		_fps = _fpsLimiter.end();
		if (frameCounter == 1000){
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}
		//my var
		/*if (frameCounter % 10 == 0){
			std::cout << _fps << std::endl;
			frameCounter = 0;
		}*/
	}
}


void MainGame::processInput(){
	SDL_Event evnt;

	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&evnt)){
		switch (evnt.type){
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;

			//нам надо сказать inputmanager где мышь находится
		case SDL_MOUSEMOTION:
			//std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
				//xrel на сколько пикселей сдвинулись с прошлого раза
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;

		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			//SDL_MOUSE_LEFT или SDL_MOUSE_RIGHT в зависимости от того какой кнопкой мыши клики. 
			//Мы считаем мышь как key, подобно a,w,d,s и т.д..
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}
	//проверка - нажатие определенной клавиши
	if (_inputManager.isKeyPressed(SDLK_w)){
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}
	if (_inputManager.isKeyPressed(SDLK_s)){
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	}
	if (_inputManager.isKeyPressed(SDLK_d)){
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}
	if (_inputManager.isKeyPressed(SDLK_a)){
	_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}
	if (_inputManager.isKeyPressed(SDLK_q)){
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	}
	if (_inputManager.isKeyPressed(SDLK_e)){
	_camera.setScale(_camera.getScale() - SCALE_SPEED);
	}
	if (_inputManager.isKeyPressed(SDLK_w) && _inputManager.isKeyPressed(SDLK_d)){
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, CAMERA_SPEED));
	}
	if (_inputManager.isKeyPressed(SDLK_w) && _inputManager.isKeyPressed(SDLK_a)){
		_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, CAMERA_SPEED));
	}
	if (_inputManager.isKeyPressed(SDLK_s) && _inputManager.isKeyPressed(SDLK_a)){
		_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, -CAMERA_SPEED));
	}
	if (_inputManager.isKeyPressed(SDLK_s) && _inputManager.isKeyPressed(SDLK_d)){
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, -CAMERA_SPEED));
	}

	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)){
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorld(mouseCoords);
		std::cout << "x:" <<mouseCoords.x << " " << "y:"<<mouseCoords.y << std::endl;
		//делаем вид что игрок всегда в центре мира. Обычно это не то что нам надо. ДУмаю, можно просто поставить _position туда
		glm::vec2 playerPosition(0.0f);
		//координаты куда мы кликнули - позиция игрока
		glm::vec2 direction = mouseCoords - playerPosition;
		//делает теорему Пифагора, деление на гипотенузу и превращает вектор в единичный
		direction = glm::normalize(direction);
		//имплайсим сразу конструктор буллета, без пляски с созданием переменной и т.д
		_bullets.emplace_back(playerPosition, direction, 5.0f, 1000);
	}
	//drawGame();
}

void MainGame::drawGame(){

	//базовая глубина, просто 1 и забить.
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //чистить экран. Тут чистит сразу два буффера
	//use the coloring
	_colorProgram.use();
	glActiveTexture(GL_TEXTURE0); //we use first texture
	//glBindTexture(GL_TEXTURE_2D, _playerTexture.id);
	//can do same shit will a lot of textures, just change GL_TEXTURE1, etc.
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation,0); //i for integer.
	//We are using glActiveTexture 0. 
	//Only 1 texture per trxture unit

	//ЗДесь мы используем для задания крутых шейдерных анимационных эффектов
	//GLint timeLocation = _colorProgram.getUniformLocation("time");
	//glUniform1f(timeLocation, _time);//cus we have only 1 float. Can be 3f, 3fv(array)
	
	//Set the camera matrix
	GLint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	//Теперь ебашим эту поеботу в GPU
	//Место матрицы, количество матриц, нужен ли transpose(хз что это), указатель, где находится данные матрицы(первый элемент)
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_spriteBatch.begin();
	glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	//инициализируется один раз, без инициализации снова и снова
	static Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("Textures/1.png");
	Bengine::Color color;
	//ДОбавить конструктор, ибо это пздц
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
		_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);

		for (int i = 0; i < _bullets.size(); i++){
			_bullets[i].draw(_spriteBatch);
		}
		//обрати внимание на размещение текстур
		//_spriteBatch.draw(pos + glm::vec4(50, 0, 0, 0), uv, texture.id, 0.0f, color);
	_spriteBatch.end();

	_spriteBatch.renderBatch();
	//draw the sprite
	/*for (int i=0; i < _sprites.size(); i++){
		_sprites[i]->draw();
	}*/
	/*for (auto & a : _sprites){
		a->draw();
	}*/
	glBindTexture(GL_TEXTURE_2D, 0);//don't forget to unbind
	//unuse the coloring
	_colorProgram.unuse();


	//Полная хрень, устарела и пользоваться не надо! Потому что медленно. ПОсілает из CPU в GPU команды по одному. МЕДЛЕННО
	//glDisableClientState(GL_COLOR_ARRAY);//для заливки
	//glBegin(GL_TRIANGLES);
	//glColor3f(1.0f,0.0f,0.0f); //3 параметра
	//glVertex2f(-0.5, -0.5);//2 параметра
	//glVertex2f(0, -0.5);
	//glVertex2f(0, 0);
	//glEnd();
	//
	
	_window.swapBuffer();
}
