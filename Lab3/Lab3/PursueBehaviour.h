#pragma once
#include <SFML/Graphics.hpp>
#include "Behaviour.h"
#include "SteeringOutput.h"
#include "Player.h"
#include "Enemy.h"
class PursueBehaviour : public Behaviour
{
public:

    PursueBehaviour(Enemy& enemy, Player& player);

    SteeringOutput GetSteering() override;

private:

    Enemy& m_enemy;
    Player& m_player;

    float maxAcceleration = 0.005f;
};

