#include "WandererBehaviour.h"
#include <cmath>

 WandererBehaviour::WandererBehaviour(Enemy& enemy, Player& player) : m_enemy(enemy), m_player(player)
{
   
}

SteeringOutput WandererBehaviour::GetSteering()
{
    //SteeringOutput steeringOutput;
    //sf::Vector2f random;
    //random.x = std::rand() % 3 - 1;
    //random.y = std::rand() % 3 - 1;
    //wanderOrientation += random * wanderRate;
    //sf::Vector2f currentOrientation = GetDirectionFacing(m_player.GetPlayerPos(), m_enemy.GetPosition());
    //targetOrientation = wanderOrientation + currentOrientation;
    //sf::Vector2f target = m_enemy.GetPosition() + wanderOffset * currentOrientation;

    //target += wanderRadius * targetOrientation;

    ////steeringOutput = face.getSteering(target);

    //steeringOutput.linear = maxAcceleration * currentOrientation;

    //SteeringOutput steeringOutput;
    //sf::Vector2f direction = steeringOutput.linear;

    //sf::Vector2f random;
    //random.x = std::rand() % 3 - 1;
    //random.y = std::rand() % 3 - 1;
    //wanderOrientation += random * wanderRate;
    //sf::Vector2f currentOrientation = GetDirectionFacing(m_player.GetPlayerPos(), m_enemy.GetPosition());
    //targetOrientation = wanderOrientation + AngleToVector(m_enemy.GetRotation());
    //sf::Vector2f target = m_enemy.GetPosition() + wanderOffset * AngleToVector(m_enemy.GetRotation());

    //target += wanderRadius * targetOrientation;

    ////steeringOutput = face.getSteering(target);

    //steeringOutput.linear = maxAcceleration * AngleToVector(m_enemy.GetRotation());

    //// Angular
    //float angle = atan2(direction.y, direction.x) * 180 / 3.14159265;
    //steeringOutput.angular = angle;
    //

    //return steeringOutput;

    // Create a steering output to store the result
    SteeringOutput steeringOutput;

    // Parameters for wandering
    float wanderRadius = 0.5f; // Radius of the wander circle
    float wanderDistance = 0.5f; // Distance ahead of the enemy to look for the circle

    // Calculate a random angle change for wandering
    float angleChange = (std::rand() % 360) * 0.5f; // You can adjust the factor to control the angle change rate

    // Update the wander angle (you can make wanderAngle a member variable)
    m_wanderAngle += angleChange;

    // Calculate the center of the wander circle
    sf::Vector2f circleCenter = m_enemy.GetPosition() + Normalise(m_enemy.GetPosition()) * wanderDistance;

    // Calculate the target position on the wander circle
    sf::Vector2f targetPosition = circleCenter + sf::Vector2f(std::cos(m_wanderAngle), std::sin(m_wanderAngle)) * wanderRadius;

    // Calculate the desired linear and angular velocities
    sf::Vector2f desiredLinear = targetPosition - m_enemy.GetPosition();
    float desiredAngular = 0.0f; // You can set this to control the enemy's angular velocity

    // Assign the desired velocities to the steering output
    steeringOutput.linear = desiredLinear;
    steeringOutput.angular = desiredAngular;

    return steeringOutput;
}


