#include "EnemyWander.h"
#include <iostream>
#include <cmath>
#include <algorithm>


EnemyWander::EnemyWander(Player& player) : player(player)
{
}

void EnemyWander::Init()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	deltaTime = clock.restart();
	SetupSprites();

}

void EnemyWander::Render(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void EnemyWander::Update(sf::RenderWindow& window)
{
	//Move();
	WrapAround(window);
	KinematicWander();
	// Calculate the rotation angle in radians
	float angle = std::atan2(velocity.y, velocity.x);
	// Convert radians to degrees for the rotation
	float degrees = angle * (180.0f / 3.14159265359f);
	sprite.setRotation(degrees + 90); // Set the rotation angle

	sprite.setPosition(currentPosition);
}

sf::Vector2f EnemyWander::GetCurrentPos()
{
	return sprite.getPosition();
}

void EnemyWander::SetupSprites()
{
	if (!enemyTexture.loadFromFile("Images\\Alien.png"))
	{
		std::cout << "problem loading Player" << std::endl;
	}
	while (randomX == 0 || randomY == 0)
	{
		randomX = static_cast<float>(std::rand() % 3 - 1);
		randomY = static_cast<float>(std::rand() % 3 - 1);
	}
	randomDirection = { randomX,randomX };

	sprite.setTexture(enemyTexture);
	sprite.setPosition(startPosition);
	sprite.setOrigin(sprite.getTextureRect().width / 2, sprite.getTextureRect().height / 2);
	sprite.setScale(0.7, 0.7);
}

void EnemyWander::Move()
{
	sprite.setPosition(currentPosition);
	sf::Vector2f movement = moveSpeed * randomDirection;
	currentPosition += movement;
}

void EnemyWander::WrapAround(sf::RenderWindow& window)
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

float EnemyWander::GetNewOrientation(float currentOrientation, sf::Vector2f currentVelocity)
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


void EnemyWander::KinematicWander()
{
	velocity = player.GetPlayerPos() - GetCurrentPos();
	velocityLength = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
	
	// normalising the velocity
	if (velocityLength == 0) return;
	velocity = { (velocity.x / velocityLength), (velocity.y / velocityLength )};
	// setting the orientation
	orientation = GetNewOrientation(orientation, velocity);
	// changing orientation based off rotation
	orientation = orientation + maxRotation /** deltaTime.asSeconds()*/ * (std::rand() % 5 - 5);
	// changing its velocity based on orientation and speed
	velocity = sf::Vector2f(-std::sin(orientation), std::cos(orientation) * maxSpeed);

	currentPosition += velocity;

}

//void Enemy::KinematicWander(float deltaTime)
//{
//	// Calculate the target orientation based on the current velocity
//	float targetOrientation = atan2(velocity.y, velocity.x);
//
//	// Calculate the rotation increment based on maxRotation
//	float rotationIncrement = maxRotation * deltaTime;
//
//	// Calculate the new orientation using linear interpolation (lerp)
//	orientation = std::lerp(orientation, targetOrientation, rotationIncrement);
//
//	// Calculate the new velocity based on the orientation
//	velocity = sf::Vector2f(-std::sin(orientation), std::cos(orientation));
//
//	
//
//	// Apply damping to gradually slow down the enemy's movement
//	velocity *= damping;
//
//	// Adjust the speed by multiplying with maxSpeed
//	velocity *= maxSpeed;
//
//	// Update the enemy's position based on the velocity
//	currentPosition += velocity;
//
//	// Call WrapAround function if needed (optional)
//	// WrapAround(window);
//}


