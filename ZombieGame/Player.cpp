#include "Player.h"
#include <SDL/SDL.h>
#include "Gun.h"
Player::Player() :_currentGun(-1)
{
}


Player::~Player()
{
}

void Player::Init(float speed, glm::vec2 position, Rasengine::InputManager* inputManager, Rasengine::Camera2D* camera,std::vector<Bullet>* bullets)
{
	_speed = speed;
	_position = position;
	_color.r = 0;
	_color.b = 185;
	_color.g = 0;
	_color.a = 255;
	_inputManager = inputManager;
	_camera = camera;
	_bullets = bullets;
}

void Player::AddGun(Gun * gun)
{
	_guns.push_back(gun);
	if (_currentGun == -1)
	{
		_currentGun = 0;
	}
}

void Player::Update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies)
{
	_direction.x = _direction.y = 0;
	if (_inputManager->isKeyPressed(SDLK_w))
	{
		_position.y += _speed;
		_direction.y = 1;
	}
	else if (_inputManager->isKeyPressed(SDLK_s))
	{
		_position.y -= _speed;
		_direction.y = -1;
	}
	if (_inputManager->isKeyPressed(SDLK_a))
	{
		_position.x -= _speed;
		_direction.x = -1;
	}
	else if (_inputManager->isKeyPressed(SDLK_d))
	{
		_position.x += _speed;
		_direction.x = 1;

	}
	if (_inputManager->isKeyPressed(SDLK_1) && _guns.size() >= 0)
		_currentGun = 0;
	else if (_inputManager->isKeyPressed(SDLK_2) && _guns.size() >= 1)
		_currentGun = 1;
	else if (_inputManager->isKeyPressed(SDLK_3) && _guns.size() >= 1)
		_currentGun = 2;

	if (_currentGun != -1)
	{
		glm::vec2 mouseCoords = _inputManager->getMouseCoords();
		mouseCoords = _camera->convertScreenToWorld(mouseCoords);

		glm::vec2 centerPosition = _position + glm::vec2(ENTITY_RADIUS);

		glm::vec2 direction = glm::normalize(mouseCoords - centerPosition);

		_guns[_currentGun]->Update(_inputManager->isKeyPressed(SDL_BUTTON_LEFT),
			centerPosition,
			direction,*_bullets
			);
	}
	CollideWithLevel(levelData);
}
