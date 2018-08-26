#include "Human.h"
#include <random>
#include <GLM/gtx/rotate_vector.hpp>
#include <ctime>

Human::Human() :_frames(0)
{
}


Human::~Human()
{
}

void Human::Init(float speed, glm::vec2 position)
{
	static std::mt19937 randomEngine(time(nullptr));

	static std::uniform_real_distribution<float>randDir(-1.0f, 1.0f);


	_health = 20;
	_color = Rasengine::ColorRGBA8(200, 0, 200, 255);
	_speed = speed;
	_position = position;
	//Get Random direction
	_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	if (_direction.length() == 0)
	{
		//Make sure direction isnt 0
		_direction = glm::vec2(1.0f, 0.0f);
	}
	_direction = glm::normalize(_direction);
}

void Human::Update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies,
	float deltaTime)
{
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float>randRotate(-10.0f, 10.0f);


	_position += _direction * _speed * deltaTime;
	///< Randomly change direction every 20 frame
	if (_frames == 120)
	{
		_direction = glm::rotate(_direction, randRotate(randomEngine));
		_frames = 0;
	}
	else
	{
		_frames++;
	}
	if (CollideWithLevel(levelData))
	{
		_direction = glm::rotate(_direction, randRotate(randomEngine));
	}
}
