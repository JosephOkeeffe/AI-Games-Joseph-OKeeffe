#include "ArriveSlow.h"

ArriveSlow::ArriveSlow(Enemy& enemy, Player& player) : m_enemy(enemy), m_player(player)
{
}

SteeringOutput ArriveSlow::GetSteering()
{
    SteeringOutput steeringOutput;

    // Linear
    sf::Vector2f direction = GetDirectionFacing(m_player.GetPlayerPos(), m_enemy.GetPosition());
    float distance = VectorLength(direction);

    if (distance < arrivalRadius)
    {
        targetSpeed = 0;
    }
    else if (distance > slowRadius)
    {
        targetSpeed = maxSpeed;
    }
    else
    {
        targetSpeed = maxSpeed * (distance / slowRadius);
    }

    sf::Vector2f targetVelocity = direction;
    targetVelocity = Normalise(targetVelocity);
    targetVelocity = targetVelocity * targetSpeed;

    float timeToTarget = 0.1;

    steeringOutput.linear = targetVelocity - sf::Vector2f{ 50,50 };
    steeringOutput.linear = steeringOutput.linear / timeToTarget;

    if (VectorLength(steeringOutput.linear) > maxAcceleration)
    {
        steeringOutput.linear = Normalise(steeringOutput.linear);
        steeringOutput.linear = steeringOutput.linear * maxAcceleration;
    }

    // Angular
    direction = steeringOutput.linear;
    float angle = atan2(direction.y, direction.x) * 180 / 3.14159265;
    steeringOutput.angular = angle;

    return steeringOutput;

}