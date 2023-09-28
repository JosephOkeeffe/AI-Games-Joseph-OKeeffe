#pragma once
#include <SFML/Graphics.hpp>
#include "Behaviour.h"
#include "SteeringOutput.h"
#include "Player.h"
#include "Enemy.h"

class ArriveFast : public Behaviour
{
public:

    ArriveFast(Enemy& enemy, Player& player);

    SteeringOutput GetSteering() override;

private:

    Enemy& m_enemy;
    Player& m_player;

    float targetSpeed;

    float arrivalRadius = 50.0f;
    float slowRadius = 200.0f;
    float maxSpeed = 7.0f;
    float maxAcceleration = 7.0f;
    float timeToTarget;

};


