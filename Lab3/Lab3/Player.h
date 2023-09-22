#pragma once
#include <SFML/Graphics.hpp> 
#include<SFML/System.hpp>

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
	sf::Vector2f Normalise(sf::Vector2f vector);
	float VectorLength(sf::Vector2f vector);

	float moveSpeed = 2;
	const float MAX_SPEED = 5;
	sf::Vector2f startPosition {100, 100};
	sf::Vector2f currentPosition {startPosition};
	sf::Vector2f velocity = { 0, 0 };
	float acceleration = 0.1f;

	float scale = 0.5f;
	float rotation;
	float rotationSpeed = 3.10f;
};

