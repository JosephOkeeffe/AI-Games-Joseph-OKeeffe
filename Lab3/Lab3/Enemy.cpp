#include <iostream>
#include "Enemy.h"

Enemy::Enemy() 
{
}

void Enemy::Init()
{
    if (!enemyTexture.loadFromFile("Images\\Spitzy.png"))
    {
        std::cout << "problem loading Player" << std::endl;
    }

    enemySprite.setTexture(enemyTexture);
    enemySprite.setPosition(200, 200);
    enemySprite.setOrigin(enemySprite.getTextureRect().width / 2, enemySprite.getTextureRect().height / 2);
    enemySprite.setScale(scale, scale);
    
}

void Enemy::Render(sf::RenderWindow& window)
{
    window.draw(enemySprite);

}

void Enemy::Update()
{

    SteeringOutput temp = currentBehaviour->GetSteering();

    enemySprite.move(temp.linear);

    std::cout << "Pos X: " << GetPosition().x << "\n";
    std::cout << "Temp X: " << temp.linear.x << "\n";
  
}

sf::Vector2f Enemy::GetPosition()
{
    return enemySprite.getPosition();
}


