#pragma once
#include <string>
#include <vector>
#include <Rasengine/include/SpriteBatch.h>

const int TILE_WIDTH = 64;
const int TILE_HEIGHT= 64;
class Level
{
public:
	//Load Level
	Level(const std::string& fileName);
	~Level();


	void Draw();
	
	int GetWidth() const { return _levelData[0].size(); }
	int GetHeight() const { return _levelData.size(); }	
	int GetNumHumans() const { return _numHumans; }
	glm::vec2 GetPlayerStartPos() const { return _startPlayerPos; }
	const std::vector<glm::vec2>& GetZombieStartPos() const { return _zombieStartPositions; }
	const std::vector<std::string>& GetLevelData() const { return _levelData; }

private:
	std::vector<std::string> _levelData;
	int _numHumans;

	Rasengine::SpriteBatch _spriteBatch; 

	glm::vec2 _startPlayerPos;
	std::vector<glm::vec2> _zombieStartPositions;
};

