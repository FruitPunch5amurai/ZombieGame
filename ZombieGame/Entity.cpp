#include "Entity.h"
#include <Rasengine/include/ResourceManager.h>
#include "Level.h"
#include <algorithm>
Entity::Entity()
{
}


Entity::~Entity()
{
}

bool Entity::CollideWithLevel(const std::vector<std::string>& levelData)
{
	std::vector<glm::vec2> collideTilePosition;

	//Fixes bug where sometimes get stuck with collision on y axis
	if (_direction.y > 0)
	{
		CheckTilePosition(levelData, collideTilePosition, _position.x, _position.y);
		CheckTilePosition(levelData, collideTilePosition, _position.x + ENTITY_WIDTH, _position.y);
		CheckTilePosition(levelData, collideTilePosition, _position.x, _position.y + ENTITY_WIDTH);
		CheckTilePosition(levelData, collideTilePosition, _position.x + ENTITY_WIDTH, _position.y + ENTITY_WIDTH);
	}
	else
	{
		CheckTilePosition(levelData, collideTilePosition, _position.x, _position.y + ENTITY_WIDTH);
		CheckTilePosition(levelData, collideTilePosition, _position.x + ENTITY_WIDTH, _position.y + ENTITY_WIDTH);
		CheckTilePosition(levelData, collideTilePosition, _position.x, _position.y);
		CheckTilePosition(levelData, collideTilePosition, _position.x + ENTITY_WIDTH, _position.y);
	}
	if (collideTilePosition.size() == 0)
	{
		return false;
	}

	for (auto i : collideTilePosition)
	{
		CollideWithTile(i);
	}
	return true;
}

bool Entity::CollideWithEntity(Entity * entity)
{
	const float MIN_DISTANCE = ENTITY_RADIUS * 2.0f;

	glm::vec2 centerPosA = _position + glm::vec2(ENTITY_RADIUS);
	glm::vec2 centerPosB = entity->GetPosition() + glm::vec2(ENTITY_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	//Give us the length between the 2 center
	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;
	
	if (collisionDepth > 0)
	{
		glm::vec2 collisionDepthVec = glm::normalize(distVec)* collisionDepth;

		_position += collisionDepthVec / 2.0f;
		entity->_position -= collisionDepthVec / 2.0f;
		return true;
	}

	return false;
}

bool Entity::ApplyDamage(float damage)
{
	_health -= damage;
	if (_health <= 0)
	{
		return true;
	}
	return false;
}


void Entity::Draw(Rasengine::SpriteBatch & spriteBatch)
{
	static int textureID = Rasengine::ResourceManager::GetTexture("Textures/circle.png").id;
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = ENTITY_WIDTH;
	destRect.w = ENTITY_WIDTH;

	spriteBatch.Draw(destRect, uvRect, textureID, 0.0f, _color);
}

void Entity::CheckTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePosition, float x, float y)
{
	//Check Corners
	glm::vec2 cornerPos = glm::ivec2(floor(x / (float)TILE_WIDTH),
		floor(y / (float)TILE_HEIGHT));
	if (cornerPos.x < 0 || cornerPos.x >= levelData[0].size() || cornerPos.y < 0 || cornerPos.y >= levelData.size())
	{
		return;
	}
	if (levelData[cornerPos.y][cornerPos.x] != '.')
	{
		collideTilePosition.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2(TILE_WIDTH / 2, TILE_HEIGHT / 2));
	}
}

//AABB Collision
void Entity::CollideWithTile(glm::vec2 tilePos)
{
	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = ENTITY_RADIUS + TILE_RADIUS;

	glm::vec2 centerPlayerPos = _position + glm::vec2(ENTITY_RADIUS);
	glm::vec2 distVec = centerPlayerPos - tilePos;

	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	//Collision detected
	if (xDepth > 0 || yDepth > 0)
	{
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f))
		{
			if (distVec.x < 0)
				_position.x -= xDepth;
			else
				_position.x += xDepth;
		}
		else
		{
			if (distVec.y < 0)
				_position.y -= yDepth;
			else
				_position.y += yDepth;
		}
	}
}
