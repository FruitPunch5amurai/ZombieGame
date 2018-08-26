#pragma once
#include <GLM/glm.hpp>
#include <vector>
#include <Rasengine/include/SpriteBatch.h>
class Human;
class Zombie;
class Entity;
const int BULLET_RADIUS = 5;

class Bullet
{
public:
	Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed);
	~Bullet();

	bool Update(const std::vector<std::string>& levelData);
	void Draw(Rasengine::SpriteBatch& spriteBatch);
	bool CollideWithEntity(Entity* entity);
	float GetDamage() const { return _damage; }

private:
	bool CollideWithWorld(const std::vector<std::string>& levelData);
	float _damage;
	glm::vec2 _position;
	glm::vec2 _direction;
	float _speed;
};

