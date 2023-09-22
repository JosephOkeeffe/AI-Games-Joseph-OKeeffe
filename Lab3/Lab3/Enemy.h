#pragma once

#include <SFML/Graphics.hpp>
#include "Behaviour.h"
#include "Player.h"
#include "SteeringOutput.h"

class Enemy
{
public:
    Enemy();
    void Init();
    void Render(sf::RenderWindow& window);
    void Update();
    sf::Vector2f GetPosition();

    inline void SetBehaviour(Behaviour* newBehaviour) 
    {
        currentBehaviour = newBehaviour;
    }

private:
    Behaviour* currentBehaviour;

    float scale = 0.5f;


    sf::Texture enemyTexture;
    sf::Sprite enemySprite;
};
