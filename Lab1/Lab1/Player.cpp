#include <iostream>
#include "Player.h"

Player::Player()
{

}

void Player::Init()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	if (!playerTexture.loadFromFile("Images\\Player.png"))
	{
		std::cout << "problem loading Player" << std::endl;
	}
	
	playerSprite.setTexture(playerTexture);
	playerSprite.setPosition(200, 200);
	playerSprite.setOrigin(playerSprite.getTextureRect().width / 2, playerSprite.getTextureRect().height / 2);
	playerSprite.setScale(0.7,0.7);
}

void Player::Render(sf::RenderWindow& window)
{
	window.draw(playerSprite);
}

void Player::Update(sf::RenderWindow& window)
{
    ChangeVelocity();
    Move();
    // Calculate the rotation angle in radians
    float angle = std::atan2(velocity.y, velocity.x);
    // Convert radians to degrees for the rotation
    float degrees = angle * (180.0f / 3.14159265359f);
    playerSprite.setRotation(degrees + 90); // Set the rotation angle
    playerSprite.setPosition(currentPosition);
    WrapAround(window);
}

sf::Vector2f Player::GetPlayerPos()
{
	return playerSprite.getPosition();
}

void Player::ChangeVelocity()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        if (velocity.y <= -MAX_SPEED) return;
        velocity.y -= acceleration;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        if (velocity.y >= MAX_SPEED) return;
       velocity.y += acceleration;
        
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        if (velocity.x <= -MAX_SPEED) return;
        velocity.x -= acceleration;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        if (velocity.x >= MAX_SPEED) return;
        velocity.x += acceleration;

    }
}

void Player::Move()
{
    // Convert rotation angle to radians
    //float angle = playerSprite.getRotation() * (3.14159265359f / 180.0f);
    //float moveX = velocity.x *std::cos(angle); // Calculate x-axis movement
    //float moveY = velocity.y *std::sin(angle); // Calculate y-axis movement

    currentPosition.x += velocity.x;
    currentPosition.y += velocity.y;

    playerSprite.setPosition(currentPosition);
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


