#pragma once

#include <SFML/Graphics.hpp> 

class Player
{
public:

	enum Direction
	{
		NORTH, 
		SOUTH, 
		EAST, 
		WEST
	};

	Player();
	void Init();
	void Render(sf::RenderWindow& window);
	void Update(sf::RenderWindow& window);
	void Move();
	void ChangeVelocity();
	void WrapAround(sf::RenderWindow& window);

	sf::Sprite playerSprite;
	sf::Texture playerTexture;

private:

	float moveSpeed = 2;
	float currentDirection = NORTH;
	sf::Vector2f startPosition {200, 200};
	sf::Vector2f currentPosition {startPosition};
	float velocity = 0.0f; 
	float acceleration = 0.1f; 
	sf::Vector2f randomDirection;

};

