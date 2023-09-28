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

    SteeringOutput steeringOutput;

    float wanderRadius = 0.5f; 
    float wanderDistance = 0.5f; 

    float angleChange = (std::rand() % 360) * 0.5f; 

    m_wanderAngle += angleChange;

    sf::Vector2f circleCenter = m_enemy.GetPosition() + Normalise(m_enemy.GetPosition()) * wanderDistance;

    sf::Vector2f targetPosition = circleCenter + sf::Vector2f(std::cos(m_wanderAngle), std::sin(m_wanderAngle)) * wanderRadius;

    sf::Vector2f desiredLinear = targetPosition - m_enemy.GetPosition();
    float desiredAngular = 0.0f; 

    steeringOutput.linear = desiredLinear;
    steeringOutput.angular = desiredAngular;

    return steeringOutput;
}


