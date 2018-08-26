#pragma once

#include "Human.h"
#include <Rasengine/include/InputManager.h>
#include <Rasengine/include/Camera.h>
#include "Bullet.h"

class Gun;

class Player :public Human
{
public:
	Player();
	~Player();

	void Update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies,
		float deltaTime) override;
	void Init(float speed,glm::vec2 position, 
		Rasengine::InputManager* inputManager,
		Rasengine::Camera2D* camera,
		std::vector<Bullet>* bullets);
	void AddGun(Gun* gun);


private:
	Rasengine::InputManager* _inputManager;
	std::vector<Gun *> _guns;
	int _currentGun;

	Rasengine::Camera2D* _camera;
	std::vector<Bullet>* _bullets;
};

