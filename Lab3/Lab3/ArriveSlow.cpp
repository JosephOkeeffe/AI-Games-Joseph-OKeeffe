#include "ArriveSlow.h"
#include <iostream>

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
        std::cout << "stop \n";

        targetSpeed = 0;
    }
    else if (distance > slowRadius)
    {
        std::cout << "MAX \n";

        targetSpeed = maxSpeed;
    }
    else
    {
        std::cout << "Slow \n";
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
  /*  direction = steeringOutput.linear;
    float angle = atan2(direction.y, direction.x) * 180 / 3.14159265;
    steeringOutput.angular = angle;*/

    return steeringOutput;

}





