#include "Bullet.h"
#include <Rasengine/include/ResourceManager.h>
#include "Entity.h"
#include "Human.h"
#include "Zombie.h"
#include "Level.h"

Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed):
	_position(position),
	_direction(direction),
	_damage(damage),
	_speed(speed)
{
}


Bullet::~Bullet()
{
}

bool Bullet::Update(const std::vector<std::string>& levelData)
{
	_position += _direction * _speed;
	return CollideWithWorld(levelData);
}

void Bullet::Draw(Rasengine::SpriteBatch & spriteBatch)
{
	glm::vec4 destRect(_position.x  + BULLET_RADIUS, 
		_position.y + BULLET_RADIUS, 
		BULLET_RADIUS * 2, 
		BULLET_RADIUS * 2);
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	Rasengine::Color color;
	color.r = 75;
	color.g = 75;
	color.b = 75;
	color.a = 255;
	spriteBatch.Draw(destRect, uvRect,Rasengine::ResourceManager::GetTexture("Textures/circle.png").id,0.0f,color);
}

bool Bullet::CollideWithEntity(Entity* entity)
{
	const float MIN_DISTANCE = ENTITY_RADIUS + BULLET_RADIUS;

	glm::vec2 centerPosA = _position;
	glm::vec2 centerPosB = entity->GetPosition() + glm::vec2(ENTITY_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	//Give us the length between the 2 center
	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0)
	{
		return true;
	}
	return false;
}

bool Bullet::CollideWithWorld(const std::vector<std::string>& levelData)
{
	glm::ivec2 gridPosition;
	gridPosition.x = floor(_position.x / (float)TILE_WIDTH);
	gridPosition.y = floor(_position.y / (float)TILE_WIDTH);

	//If we are outside the world, just return
	if (gridPosition.x < 0 || gridPosition.x >= levelData[0].size() 
		|| gridPosition.y < 0 || gridPosition.y >= levelData.size())
	{
		return true;
	}
	return (levelData[gridPosition.y][gridPosition.x] != '.');

}
