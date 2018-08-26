#pragma once
#include <Sprite.h>
#include <SDL/SDL.h>
#include <GLSLProgram.h>
#include <GLTexture.h>
#include <Window.h>
#include <Camera.h>
#include <vector>
#include <InputManager.h>
#include <SpriteBatch.h>
#include <Timing.h>
#include "Level.h"
#include "Player.h"
#include "Bullet.h"


enum class GameState {
	PLAY,
	EXIT
};

class Zombie;

class MainGame
{
public:
    MainGame();
    ~MainGame();

    /// Runs the game
    void run();

private:
    /// Initializes the core systems
    void initSystems();

	//Initializes Level and sets up stuff
	void InitLevel(std::string levelPath, int currentLevel);
    /// Initializes the shaders
    void initShaders();

    /// Main game loop for the program
    void gameLoop();

	///Check Win Condition
	void CheckVictory();
    /// Handles input processing
    void processInput();

	///Updates all entities
	void UpdateEntities(float deltaTime);

	///Update all bullets
	void UpdateBullets(float deltaTime);

    /// Renders the game
    void drawGame();

private:
    /// Member Variables
    Rasengine::Window _window; ///< The game window
	Rasengine::GLSLProgram _textureProgram; ///< The shader program
	Rasengine::InputManager _inputManager; ///< Handles input
	Rasengine::Camera2D _camera; ///< Main Camera
	Rasengine::SpriteBatch _entitySpriteBatch; ///< SpriteBatch for entities

	std::vector<Level*> _levels;
	std::vector<Human*> _humans;
	std::vector<Zombie*> _zombies;
	std::vector<Bullet> _bullets;

	Player* _player;

	int _humansKilledByPlayer;
	int _zombiesKilledByPlayer;

	int _screenWidth;
	int _screenHeight;
	int _fps;

	int _currentLevel;
	GameState _gameState;
};

