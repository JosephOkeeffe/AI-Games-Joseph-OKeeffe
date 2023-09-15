#include <iostream>
#include "Player.h"

Player::Player()
{

}

void Player::Init()
{
	std::srand(time(nullptr));

	if (!playerTexture.loadFromFile("Images\\Player.png"))
	{
		std::cout << "problem loading Player" << std::endl;
	}

	randomDirection = { static_cast<float>(std::rand() % 3 - 1),
					static_cast<float>(std::rand() % 3 - 1) };

	playerSprite.setTexture(playerTexture);
	playerSprite.setPosition(200, 200);
	playerSprite.setOrigin(playerSprite.getTextureRect().width / 2, playerSprite.getTextureRect().height / 2);
	playerSprite.setScale(1,1);
}

void Player::Render(sf::RenderWindow& window)
{
	window.draw(playerSprite);
}

void Player::Update(sf::RenderWindow& window)
{
    ChangeVelocity(); 
    Move();
    playerSprite.setPosition(currentPosition);
	WrapAround(window);
}

void Player::ChangeVelocity()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        velocity += acceleration;
        if (velocity > 5)
        {
            velocity = 5;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        velocity -= acceleration;

        if (velocity < -5)
        {
            velocity = -5;
        }
    }
}

void Player::Move()
{
    float angle = playerSprite.getRotation() * (3.14159265359f / 180.0f); // Convert rotation angle to radians
    float moveX = velocity * std::sin(angle);
    float moveY = -velocity * std::cos(angle);

    currentPosition.x += moveX;
    currentPosition.y += moveY;

    playerSprite.setPosition(currentPosition);

	playerSprite.setPosition(currentPosition);
	sf::Vector2f movement = moveSpeed * randomDirection;
	currentPosition += movement;
}
void Player::WrapAround(sf::RenderWindow& window)
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


