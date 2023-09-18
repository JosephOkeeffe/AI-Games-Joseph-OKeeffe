#pragma once

#include <SFML/Graphics.hpp> 

class Player
{
public:

	Player();
	void Init();
	void Render(sf::RenderWindow& window);
	void Update(sf::RenderWindow& window);

	sf::Vector2f GetPlayerPos();


	sf::Sprite playerSprite;
	sf::Texture playerTexture;


private:

	void Move();
	void ChangeVelocity();
	void WrapAround(sf::RenderWindow& window);

	float moveSpeed = 2;
	const float MAX_SPEED = 5;
	sf::Vector2f startPosition {100, 100};
	sf::Vector2f currentPosition {startPosition};
	sf::Vector2f velocity = { 0, 0 };
	float acceleration = 0.1f; 




};

