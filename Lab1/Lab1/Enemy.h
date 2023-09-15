#pragma once

#include <SFML/Graphics.hpp> 

class Enemy
{
public:
	Enemy();
	void Init();
	void Render(sf::RenderWindow& window);
	void Update(sf::RenderWindow& window);
	void Move();
	void WrapAround(sf::RenderWindow& window);

	sf::Sprite enemySprite;
	sf::Texture enemyTexture;


private:

	float moveSpeed = 2;
	sf::Vector2f startPosition {500, 500};
	sf::Vector2f currentPosition {startPosition};
	sf::Vector2f randomDirection;
};

