#include "MainGame.h"

#include <SDL/SDL.h>
#include <ImageLoader.h>
#include <Rasengine.h>
#include <Vertex.h>
#include <ResourceManager.h>
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include<Rasengine/include/Timing.h>
#include <random>
#include <ctime>
#include <Rasengine/include/Errors.h>
#include "Zombie.h"
#include "Gun.h"

const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;


MainGame::MainGame() :
	_screenWidth(1024),
	_screenHeight(768),
	_gameState(GameState::PLAY),
	_fps(0.0f),
	_player(nullptr),
	_humansKilledByPlayer(0),
	_zombiesKilledByPlayer(0) {
}

MainGame::~MainGame()
{
	for (int i = 0; i < _levels.size(); i++)
	{
		delete _levels[i];
	}
}

void MainGame::run() {
	initSystems();

	InitLevel("Levels/level1.txt", 0);

	gameLoop();
}


void MainGame::initSystems()
{
	Rasengine::Init();
	_window.CreateSDLWindow("ZombieGame", _screenWidth, _screenHeight, 0);
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

	initShaders();

	_entitySpriteBatch.Init();

	_camera.Init(_screenWidth, _screenHeight);
}

void MainGame::InitLevel(std::string levelPath, int currentLevel)
{
	//Level 1
	_levels.push_back(new Level(levelPath));
	_currentLevel = currentLevel;

	_player = new Player();
	_player->Init(10.0f, _levels[currentLevel]->GetPlayerStartPos(), &_inputManager, &_camera, &_bullets);

	_humans.push_back(_player);

	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int>randX(2.0f, _levels[currentLevel]->GetWidth() - 2);
	std::uniform_int_distribution<int>randY(2.0f, _levels[currentLevel]->GetHeight() - 2);

	for (int i = 0; i < _levels[currentLevel]->GetNumHumans(); i++)
	{
		_humans.push_back(new Human);
		glm::vec2 pos(randX(randomEngine)* TILE_WIDTH, randY(randomEngine)* TILE_HEIGHT);
		_humans.back()->Init(HUMAN_SPEED, pos);
	}
	//Add Zombies
	const std::vector<glm::vec2>& zombiePositions = _levels[_currentLevel]->GetZombieStartPos();
	for (int i = 0; i < zombiePositions.size(); i++)
	{
		_zombies.push_back(new Zombie);
		_zombies.back()->Init(ZOMBIE_SPEED, zombiePositions[i]);
	}

	//Set up player guns
	_player->AddGun(new Gun("Magnum", 30, 1, 0.1f, 30.0f, 20.0f));
	_player->AddGun(new Gun("Shoutgun", 60, 12, 0.3f, 4.0f, 20.0f));
	_player->AddGun(new Gun("Auto Rifle", 5, 1, 0.2f, 20.0f, 20.0f));

}

void MainGame::initShaders() {
	// Compile our color shader
	_textureProgram.CompileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	_textureProgram.AddAttribute("vertexPosition");
	_textureProgram.AddAttribute("vertexColor");
	_textureProgram.AddAttribute("vertexUV");
	_textureProgram.LinkShaders();
}

void MainGame::gameLoop() {

	const float DESIRED_FPS = 60.0f;
	const int MAX_PHYSICS_STEPS = 6; ///< Protects from spiral of death...
	Rasengine::FPSLimiter fpsLimiter;
	fpsLimiter.Init(DESIRED_FPS);

	//Set Camera Scale
	const float CAMERA_SCALE = 1.0f / 4.0f;
	_camera.SetScale(CAMERA_SCALE);

	//Calculate variables for Semi Fixed Time Step
	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SECOND/ DESIRED_FPS; ///< Number of milliseconds it takes to complete one frame
	const float MAX_DELTA_TIME = 1.0f;
	float previousTicks = SDL_GetTicks();

	while (_gameState == GameState::PLAY)
	{
		fpsLimiter.Begin();

		//Semi Fixed Time Step
		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - previousTicks;
		previousTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		CheckVictory();

		_inputManager.Update();

		processInput();

		int i = 0;		///< So that we arent getting stuck in while loop when CPU Intensive
		//Semi Fixed Time Step
		while(totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
		{
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			
			UpdateEntities(deltaTime);

			UpdateBullets(deltaTime);

			totalDeltaTime -= deltaTime;

			i++;
		}
		_camera.SetPosition(_player->GetPosition());

		_camera.Update();

		drawGame();

		_fps = fpsLimiter.End();
	}
}

void MainGame::CheckVictory()
{
	//If all zombies are dead;
	if (_zombies.empty())
	{
		std::printf("**** You Win ****\n You killed %d humans and %d zombies.\n There are %d/%d civilians remaining",
			_humansKilledByPlayer,_zombiesKilledByPlayer,_humans.size() - 1,_levels[_currentLevel]->GetNumHumans());
		Rasengine::FatalError("");
	}
}

void MainGame::processInput() {
	SDL_Event evnt;

	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			//Exit the game here!
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.KeyPress(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.KeyRelease(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.KeyPress(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.KeyRelease(evnt.button.button);
			break;
		}

	}

}

void MainGame::UpdateEntities(float deltaTime)
{
	//Update Humans
	for (int i = 0; i < _humans.size(); i++)
	{
		_humans[i]->Update(_levels[_currentLevel]->GetLevelData(),
			_humans,
			_zombies,
			deltaTime);
		//Update Collisions
		for (int j = i + 1; j < _humans.size(); j++)
		{
			_humans[i]->CollideWithEntity(_humans[j]);
		}
	}
	//Update Zombies
	for (int i = 0; i < _zombies.size(); i++)
	{
		_zombies[i]->Update(_levels[_currentLevel]->GetLevelData(),
			_humans,
			_zombies,
			deltaTime);
		//Update Collisions
		for (int j = i + 1; j < _zombies.size(); j++)
		{
			_zombies[i]->CollideWithEntity(_zombies[j]);
		}
		for (int j = 1; j < _humans.size(); j++)
		{
			if (_zombies[i]->CollideWithEntity(_humans[j]))
			{
				//Add new Zombie
				_zombies.push_back(new Zombie);
				_zombies.back()->Init(ZOMBIE_SPEED, _humans[j]->GetPosition());

				//Delete Human
				delete _humans[j];
				_humans[j] = _humans.back();
				_humans.pop_back();
			}
		}

		//Collide with player
		if (_zombies[i]->CollideWithEntity(_player))
		{
			Rasengine::FatalError("\nYOU LOSE");
		}
	}
}

void MainGame::UpdateBullets(float deltaTime)
{
	//Update and collide with world
	for (int i = 0; i < _bullets.size();)
	{
		if (_bullets[i].Update(_levels[_currentLevel]->GetLevelData(), deltaTime))
		{
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		else {
			i++;
		}
	}
	bool wasBulletRemoved;

	//Collide with humans and zombies
	for (int i = 0; i < _bullets.size(); i++)
	{
		wasBulletRemoved = false;
		for (int j = 0; j < _zombies.size();)
		{
			//Check collision with zombie
			if (_bullets[i].CollideWithEntity(_zombies[j]))
			{
				//Damage Zombie
				if (_zombies[j]->ApplyDamage(_bullets[i].GetDamage()))
				{
					delete _zombies[j];
					_zombies[j] = _zombies.back();
					_zombies.pop_back();
					_zombiesKilledByPlayer++;
				}
				else {
					j++;
				}
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				wasBulletRemoved = true;
				i--; //make sure we dont skip a bullet
				break;
			}
			else {
				j++;
			}
		}

		//Loop through Humans
		if (wasBulletRemoved == false)
		{
			for (int j = 1; j < _humans.size();)
			{
				//Check collision with zombie
				if (_bullets[i].CollideWithEntity(_humans[j]))
				{
					//Damage human
					if (_humans[j]->ApplyDamage(_bullets[i].GetDamage()))
					{
						delete _humans[j];
						_humans[j] = _humans.back();
						_humans.pop_back();
						_humansKilledByPlayer++;
					}
					else {
						j++;
					}
					_bullets[i] = _bullets.back();
					_bullets.pop_back();
					wasBulletRemoved = true;
					i--; //make sure we dont skip a bullet
					break;
				}
				else {
					j++;
				}
			}
		}
	}
}

void MainGame::drawGame() {
	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_textureProgram.Use();

	//Draw code goes here
	glActiveTexture(GL_TEXTURE0);

	//Make sure shader uses texture 0
	GLint texutreUniform = _textureProgram.GetUnifromLocation("mySampler");
	glUniform1i(texutreUniform, 0);

	glm::mat4 projectionMatrix = _camera.GetCameraMatrix();

	GLint pUniform = _textureProgram.GetUnifromLocation("transformationMatrix");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	_levels[_currentLevel]->Draw();
	_entitySpriteBatch.Begin();
	//Draw Player
	for (auto human : _humans)
	{
		human->Draw(_entitySpriteBatch);
	}
	for (auto zombie : _zombies)
	{
		zombie->Draw(_entitySpriteBatch);
	}

	//Draw the bullets
	for (int i = 0; i < _bullets.size(); i++)
	{
		_bullets[i].Draw(_entitySpriteBatch);
	}

	_entitySpriteBatch.End();

	_entitySpriteBatch.RenderBatch();

	_textureProgram.Unuse();

	//Swap our buffer and draw everything to the screen!
	_window.SwapOpenGLBuffer();
}