#include "Level.h"
#include <fstream>
#include <iostream>
#include <Rasengine/include/Errors.h>
#include <Rasengine/include/ResourceManager.h>
Level::Level(const std::string& fileName)
{

	std::fstream file;
	file.open(fileName);
	//error checking
	if (file.fail())
	{
		Rasengine::FatalError("Failed to open " + fileName);
	}

	//Throw away first string in tmp
	std::string tmp;
	file >> tmp >> _numHumans;
	std::getline(file, tmp); // throw away rest of first line;
	while (std::getline(file, tmp))
	{
		_levelData.push_back(tmp);
	}
	_spriteBatch.Init();
	_spriteBatch.Begin();

	glm::vec4 uvRect(0.0f,0.0f,1.0f,1.0f);
	Rasengine::Color color;
	color.r = 255;
	color.b = 255;
	color.g = 255;
	color.a = 255;

	for (int y= 0; y < _levelData.size(); y++)
	{
		for (int x = 0; x< _levelData[y].size(); x++)
		{
			char tile = _levelData[y][x];

			//Get Dest Rect
			glm::vec4 destRect(x * TILE_WIDTH, y*TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT);

			switch (tile)
			{
			case 'R':
				_spriteBatch.Draw(destRect,
					uvRect, Rasengine::ResourceManager::GetTexture("Textures/red_bricks.png").id,
					0.0f,
					color);
				break;
			case 'G':
				_spriteBatch.Draw(destRect,
					uvRect, Rasengine::ResourceManager::GetTexture("Textures/glass.png").id,
					0.0f,
					color);
				break;
			case 'L':
				_spriteBatch.Draw(destRect,
					uvRect, Rasengine::ResourceManager::GetTexture("Textures/grey_bricks.png").id,
					0.0f,
					color);
				break;
			case '@':
				_levelData[y][x] = '.';
				_startPlayerPos.x = x * TILE_WIDTH;
				_startPlayerPos.y = y * TILE_HEIGHT;
				break;
			case 'Z':
				_levelData[y][x] = '.';
				_zombieStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_HEIGHT);
				break;
			case '.':
				break;
			default:
				std::printf("Unexpected symbol %c at (%d,%d)", tile, x, y);
				break;
			}
		}
	}
	_spriteBatch.End();	
}


Level::~Level()
{
}

void Level::Draw()
{
	_spriteBatch.RenderBatch();
}
