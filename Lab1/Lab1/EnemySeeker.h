#pragma once

#include <SFML/Graphics.hpp> 
#include "Player.h"

class EnemySeeker
{
public:
	EnemySeeker(Player& player);
	void Init();
	void Render(sf::RenderWindow& window);
	void Update(sf::RenderWindow& window);
	sf::Vector2f GetEnemyPos();

	sf::Sprite sprite;
	sf::Texture enemyTexture;


private:

	void SetupSprites();
	void WrapAround(sf::RenderWindow& window);

	float GetNewOrientation(float currentOrientation, sf::Vector2f currentVelocity);
	sf::Vector2f NormaliseVelocity();
	void KinematicSeek();
	void KinematicArrive();
	

	float moveSpeed = 0.5;
	float maxSpeed = 2;
	sf::Vector2f startPosition{ 742, 721 };
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

