#pragma once

#include <SFML/Graphics.hpp> 
#include "Player.h"

class EnemyWander
{
public:
	EnemyWander(Player& player);
	void Init();
	void Render(sf::RenderWindow& window);
	void Update(sf::RenderWindow& window);
	sf::Vector2f GetCurrentPos();
	


	sf::Sprite sprite;
	sf::Texture enemyTexture;


private:

	void SetupSprites();
	void Move();
	void WrapAround(sf::RenderWindow& window);

	float GetNewOrientation(float currentOrientation, sf::Vector2f currentVelocity);
	void KinematicWander();
	//void KinematicWander(float deltaTime);


	float moveSpeed = 0.5;
	float maxSpeed = 2;
	sf::Vector2f startPosition {500, 500};
	sf::Vector2f currentPosition {startPosition};
	sf::Vector2f randomDirection;
	float randomX = 0;
	float randomY = 0;

	// Kinematic wander
	float orientation = 0.0f;
	sf::Vector2f velocity{ 0,0 };
	float rotation;
	float velocityLength;
	float maxRotation = 50;


	// Time
	sf::Clock clock;
	sf::Time deltaTime;
	float rotationIncrement;
	float wanderStrength = 5.0f;
	const float damping = 0.5f;
	Player& player;
};

