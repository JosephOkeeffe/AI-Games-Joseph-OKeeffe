#include "EnemySeeker.h"
#include <iostream>


EnemySeeker::EnemySeeker(Player& player) : player(player)
{
}

void EnemySeeker::Init()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	SetupSprites();

}

void EnemySeeker::Render(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void EnemySeeker::Update(sf::RenderWindow& window)
{
	WrapAround(window);
	KinematicSeek();
	KinematicArrive();

	float angle = std::atan2(velocity.y, velocity.x);
	float degrees = angle * (180.0f / 3.14159265359f);
	sprite.setRotation(degrees + 90); 

	sprite.setPosition(currentPosition);
}

sf::Vector2f EnemySeeker::GetEnemyPos()
{
	return sprite.getPosition();
}

void EnemySeeker::SetupSprites()
{
	if (!enemyTexture.loadFromFile("Images\\Seeker.png"))
	{
		std::cout << "problem loading Player" << std::endl;
	}
	sprite.setTexture(enemyTexture);
	sprite.setPosition(startPosition);
	sprite.setOrigin(sprite.getTextureRect().width / 2, sprite.getTextureRect().height / 2);
	sprite.setScale(1, 1);
}

void EnemySeeker::WrapAround(sf::RenderWindow& window)
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

float EnemySeeker::GetNewOrientation(float currentOrientation, sf::Vector2f currentVelocity)
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

sf::Vector2f EnemySeeker::NormaliseVelocity()
{
	if (velocityLength == 0) return sf::Vector2f(0,0);

	velocity = { (velocity.x / velocityLength), (velocity.y / velocityLength) };
	return velocity;
}


void EnemySeeker::KinematicSeek()
{
	velocity = player.GetPlayerPos() - GetEnemyPos();
	velocityLength = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
	NormaliseVelocity();
	velocity = velocity * maxSpeed;
	orientation = GetNewOrientation(orientation, velocity);

	currentPosition += velocity;
}
void EnemySeeker::KinematicArrive()
{
	velocity = player.GetPlayerPos() - GetEnemyPos();
	if (velocityLength < radius)
	{
		std::cout << "Arrived \n";
		return;
	}
	else
	{
		velocity = velocity / timeToTarget;
		if (velocityLength > maxSpeed)
		{
			NormaliseVelocity();
			velocity = velocity * maxSpeed;

		}
	}
	orientation = GetNewOrientation(orientation, velocity);


}



