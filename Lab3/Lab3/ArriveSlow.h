#pragma once
#include <SFML/Graphics.hpp>
#include "Behaviour.h"
#include "SteeringOutput.h"
#include "Player.h"
#include "Enemy.h"

class ArriveSlow : public Behaviour
{
public:

    ArriveSlow(Enemy& enemy, Player& player);

    SteeringOutput GetSteering() override;

private:

    Enemy& m_enemy;
    Player& m_player;

    float targetSpeed;

    float arrivalRadius = 10.0f;
    float slowRadius = 50.0f;
    float maxSpeed = 0.5f;
    float maxAcceleration = 0.5f;
    float timeToTarget;
};

