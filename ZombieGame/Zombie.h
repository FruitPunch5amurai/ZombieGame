#pragma once

#include "Entity.h"

class Zombie : public Entity
{
public:
	Zombie();
	~Zombie();
	virtual void Update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies, float deltaTime) override;
	void Init(float speed, glm::vec2 position);
private:

	Human * GetNearestHuman(std::vector<Human*>& humans);

};

