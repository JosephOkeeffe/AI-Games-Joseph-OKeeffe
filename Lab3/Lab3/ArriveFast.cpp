#include "ArriveFast.h"

ArriveFast::ArriveFast(Enemy& enemy, Player& player) : m_enemy(enemy), m_player(player)
{
}

SteeringOutput ArriveFast::GetSteering()
{
    SteeringOutput steeringOutput;

    // Calculate the direction from the enemy to the player
    sf::Vector2f direction = m_player.GetPlayerPos() - m_enemy.GetPosition();
    float distance = VectorLength(direction);

    // Check if the enemy is within the stop radius
    if (distance < 20)
    {
        // Stop the enemy
        steeringOutput.linear = sf::Vector2f(0.0f, 0.0f);
        steeringOutput.angular = 0.0f;
    }
    else
    {
        float targetSpeed;

        if (distance < slowRadius)
        {
            targetSpeed = maxSpeed * (distance / slowRadius);
        }
        else
        {
            targetSpeed = maxSpeed;
        }

        sf::Vector2f targetVelocity = Normalise(direction) * targetSpeed;

        sf::Vector2f acceleration = (targetVelocity - m_enemy.GetVelocity()) / timeToTarget;

        if (VectorLength(acceleration) > maxAcceleration)
        {
            acceleration = Normalise(acceleration) * maxAcceleration;
        }

        steeringOutput.linear = acceleration;
        steeringOutput.angular = 0.0f;
    }

    return steeringOutput;
}



