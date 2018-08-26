#include "Human.h"
#include "Zombie.h"



Zombie::Zombie()
{
}


Zombie::~Zombie()
{
}

void Zombie::Update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies,
	float deltaTime)
{
	Human* human = GetNearestHuman(humans);
	if (human != nullptr)
	{
		glm::vec2 direction = glm::normalize(human->GetPosition() - _position);
		_position += direction * _speed * deltaTime;
	}


	CollideWithLevel(levelData);
}

void Zombie::Init(float speed, glm::vec2 position)
{
	_health = 150;
	_speed = speed;
	_position = position;
	_color = Rasengine::ColorRGBA8(0, 160, 0, 255);
}

Human * Zombie::GetNearestHuman(std::vector<Human*>& humans)
{
	Human* closestHuman = nullptr;
	float smallestDistance = 99999999999;
	for (int i = 0; i < humans.size(); i++)
	{
		glm::vec2 distVec = humans[i]->GetPosition() - _position;
		float distance = glm::length(distVec);
		if (distance < smallestDistance)
		{
			smallestDistance = distance;
			closestHuman = humans[i];
		}
	}
	return closestHuman;
}
