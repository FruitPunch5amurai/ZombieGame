#pragma once

#include <GLM/glm.hpp>
#include <Rasengine/include/ImageLoader.h>
#include <Rasengine/include/SpriteBatch.h>
class Zombie;
class Human;

const float ENTITY_WIDTH = 60;
const float ENTITY_RADIUS = ENTITY_WIDTH / 2.0f;

class Entity
{
public:
	Entity();
	virtual ~Entity();


	virtual void Update(const std::vector<std::string>& levelData,
						std::vector<Human*>& humans,
						std::vector<Zombie*>& zombies) = 0;
	bool CollideWithLevel(const std::vector<std::string>& levelData);

	bool CollideWithEntity(Entity* entity);

	bool ApplyDamage(float damage);

	glm::vec2 GetPosition()const { return _position; }

	void Draw(Rasengine::SpriteBatch& spriteBatch);
protected:
	void CheckTilePosition(const std::vector<std::string>& levelData,
							std::vector<glm::vec2>& collideTilePosition,
							float x,float y);
	void CollideWithTile(glm::vec2 tilePos);
	glm::vec2 _position;
	float _speed;
	glm::vec2 _direction;
	Rasengine::Color _color;

	float _health;
};

