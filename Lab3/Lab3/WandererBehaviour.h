#pragma once
#include "Behaviour.h"
#include "SteeringOutput.h"
#include "Player.h"
#include "Enemy.h"

class WandererBehaviour : public Behaviour
{
public:

    WandererBehaviour(Enemy& enemy, Player& player);


    SteeringOutput GetSteering() override;

private:

    Enemy& m_enemy;
    Player& m_player;

    sf::Vector2f wanderOrientation;
    sf::Vector2f targetOrientation;
    float wanderOffset = 5;
    float wanderRadius = 25;
    float wanderRate = 100;
    float maxAcceleration = 0.001f;

    float m_wanderAngle;
};

