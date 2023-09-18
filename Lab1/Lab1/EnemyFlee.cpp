#include "EnemyFlee.h"
#include <iostream>


EnemyFlee::EnemyFlee(Player& player) : player(player)
{
}

void EnemyFlee::Init()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	SetupSprites();

}

void EnemyFlee::Render(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void EnemyFlee::Update(sf::RenderWindow& window)
{
	WrapAround(window);
	KinematicFlee();
	float angle = std::atan2(velocity.y, velocity.x);
	float degrees = angle * (180.0f / 3.14159265359f);
	sprite.setRotation(degrees + 90); // Set the rotation angle

	sprite.setPosition(currentPosition);
}

sf::Vector2f EnemyFlee::GetCurrentPos()
{
	return sprite.getPosition();
}

void EnemyFlee::SetupSprites()
{
	if (!enemyTexture.loadFromFile("Images\\Flee.png"))
	{
		std::cout << "problem loading Player" << std::endl;
	}
	sprite.setTexture(enemyTexture);
	sprite.setPosition(startPosition);
	sprite.setOrigin(sprite.getTextureRect().width / 2, sprite.getTextureRect().height / 2);
	sprite.setScale(0.1, 0.1);
}

void EnemyFlee::WrapAround(sf::RenderWindow& window)
{
	if (currentPosition.x > window.getSize().x)
	{
		currentPosition.x = 150;
	}
	else if (currentPosition.x < 0)
	{
		currentPosition.x = window.getSize().x - 150;
	}
	if (currentPosition.y > window.getSize().y)
	{
		currentPosition.y = 150;
	}
	else if (currentPosition.y < 0)
	{
		currentPosition.y = window.getSize().y - 150;
	}
}

float EnemyFlee::GetNewOrientation(float currentOrientation, sf::Vector2f currentVelocity)
{
	if (velocityLength > 0)
	{
		return atan2(currentPosition.y, currentPosition.x);
	}
	else
	{
		return currentOrientation;
	}
}

sf::Vector2f EnemyFlee::NormaliseVelocity()
{
	if (velocityLength == 0) return sf::Vector2f(0, 0);

	velocity = { (velocity.x / velocityLength), (velocity.y / velocityLength) };
	return velocity;
}


void EnemyFlee::KinematicFlee()
{
	velocity = player.GetPlayerPos() - GetCurrentPos();
	velocityLength = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

	// normalising the velocity
	NormaliseVelocity();
	velocity = velocity * maxSpeed;
	// setting the orientation
	orientation = GetNewOrientation(orientation, velocity);

	currentPosition -= velocity;

}