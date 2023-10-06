#include "SeekBehaviour.h"
#include "SteeringOutput.h"
#include <iostream>

SeekBehaviour::SeekBehaviour(Enemy& enemy,Player& player) : m_enemy(enemy), m_player(player)
{
}

SteeringOutput SeekBehaviour::GetSteering()
{
    SteeringOutput steeringOutput;

    // Linear
    sf::Vector2f desiredVelocity;
    desiredVelocity = m_player.GetPlayerPos() - m_enemy.GetPosition();
    desiredVelocity = Normalise(desiredVelocity);
    steeringOutput.linear = desiredVelocity * maxAcceleration;

    // Angular
    sf::Vector2f direction = steeringOutput.linear;
    float angle = atan2(direction.y, direction.x) * 180 / 3.14159265;
    steeringOutput.angular = angle;

    return steeringOutput;
}
