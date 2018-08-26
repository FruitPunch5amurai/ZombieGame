#pragma once
#include "Entity.h"

class Human :public Entity
{
public:
	Human();
	virtual ~Human();
	void Init(float speed, glm::vec2 position);
	virtual void Update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies, float deltaTime) override;
private:
	int _frames;

};

