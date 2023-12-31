//#pragma once
//#include "GenericEnemy.h"
//
//class EnemyFlee : public GenericEnemy
//{
//	void Behaviour(float dt) override;
//};
//

#pragma once

#include <SFML/Graphics.hpp> 
#include "Player.h"

class EnemyFlee
{
public:
	EnemyFlee(Player& player);
	void Init();
	void Render(sf::RenderWindow& window);
	void Update(sf::RenderWindow& window);
	sf::Vector2f GetCurrentPos();

	sf::Sprite sprite;
	sf::Texture enemyTexture;

private:

	void SetupSprites();
	void WrapAround(sf::RenderWindow& window);

	float GetNewOrientation(float currentOrientation, sf::Vector2f currentVelocity);
	sf::Vector2f NormaliseVelocity();
	void KinematicFlee();

	float moveSpeed = 0.05;
	float maxSpeed = 0.5;
	sf::Vector2f startPosition{ 150, 150 };
	sf::Vector2f currentPosition{ startPosition };
	float randomX = 0;
	float randomY = 0;

	// Kinematic wander
	float orientation = 0.0f;
	sf::Vector2f velocity{ 0,0 };
	float velocityLength = 0;
	float maxRotation = 50;

	float radius = 5;
	float timeToTarget = 0.25;


	Player& player;
};
