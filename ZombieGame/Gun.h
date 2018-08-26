#pragma once
#include <vector>
#include <string>
#include <GLM/glm.hpp>
#include "Bullet.h"
class Gun
{
public:
	Gun(std::string name, int fireRate, int bulletsPerShot,float spread,float bulletDamage, float bulletSpeed);
	~Gun();

	void Update(bool isMouseDown,const glm::vec2& position , const glm::vec2& direction, std::vector<Bullet>& bullets);

private:
	std::string _name;
	int _fireRate; //< Fire rate in terms of frames
	int _bulletsPerShot; ///< How many bullets fired at a time
	float _spread; ///< Accuracy

	float _bulletDamage;

	float _bulletSpeed;

	int _frameCounter;

	void Fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet> &bullets);
};

