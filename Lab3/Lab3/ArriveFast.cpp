#include "ArriveFast.h"
#include <iostream>

ArriveFast::ArriveFast(Enemy& enemy, Player& player) : m_enemy(enemy), m_player(player)
{
}

SteeringOutput ArriveFast::GetSteering()
{
    SteeringOutput steeringOutput;


    sf::Vector2f direction = GetDirectionFacing(m_player.GetPlayerPos(), m_enemy.GetPosition());
    float distance = VectorLength(direction);

    if (distance < arrivalRadius)
    {
        m_enemy.ChangeColor(sf::Color(204, 0, 0));

        if (targetSpeed <= 0)
        {
            targetSpeed = 0;
        } 
        else
        {
            targetSpeed--;
        }
        

    }
    else if (distance > slowRadius)
    {
        m_enemy.ChangeColor(sf::Color::White);
        targetSpeed = maxSpeed;
    }
    else
    {
        m_enemy.ChangeColor(sf::Color(204, 102, 255));
        targetSpeed = maxSpeed * (distance / slowRadius);
    }

    sf::Vector2f targetVelocity = direction;
    targetVelocity = Normalise(targetVelocity);
    targetVelocity = targetVelocity * targetSpeed;

    float timeToTarget = 0.1;

    steeringOutput.linear = targetVelocity - m_enemy.GetVelocity(); // current vec of enemy???????
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






