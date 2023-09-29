#include "WandererBehaviour.h"
#include <cmath>

 WandererBehaviour::WandererBehaviour(Enemy& enemy, Player& player) : m_enemy(enemy), m_player(player)
{
   
}

SteeringOutput WandererBehaviour::GetSteering()
{
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


