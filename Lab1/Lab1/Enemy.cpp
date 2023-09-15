#include "Enemy.h"
#include <iostream>

Enemy::Enemy()
{
}

void Enemy::Init()
{

	std::srand(time(nullptr));

	if (!enemyTexture.loadFromFile("Images\\Alien.png"))
	{
		std::cout << "problem loading Player" << std::endl;
	}
	randomDirection = { static_cast<float>(std::rand() % 3 - 1),
						static_cast<float>(std::rand() % 3 - 1)};

	enemySprite.setTexture(enemyTexture);
	enemySprite.setPosition(200, 200);
	enemySprite.setOrigin(enemySprite.getTextureRect().width / 2, enemySprite.getTextureRect().height / 2);
	enemySprite.setScale(1, 1);
}

void Enemy::Render(sf::RenderWindow& window)
{
	window.draw(enemySprite);
}

void Enemy::Update(sf::RenderWindow& window)
{
	Move();
	WrapAround(window);
}

void Enemy::Move()
{
	
	enemySprite.setPosition(currentPosition);
	sf::Vector2f movement = moveSpeed * randomDirection;
	currentPosition += movement;
}

void Enemy::WrapAround(sf::RenderWindow& window)
{
	if (currentPosition.x > window.getSize().x)
	{
		currentPosition.x = 0;
	}
	else if (currentPosition.x < 0)
	{
		currentPosition.x = window.getSize().x;
	}
	if (currentPosition.y > window.getSize().y)
	{
		currentPosition.y = 0;
	}
	else if (currentPosition.y < 0)
	{
		currentPosition.y = window.getSize().y;
	}
}
