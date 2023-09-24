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

    SteeringOutput steeringOutput;
    sf::Vector2f direction = steeringOutput.linear;

    sf::Vector2f random;
    random.x = std::rand() % 3 - 1;
    random.y = std::rand() % 3 - 1;
    wanderOrientation += random * wanderRate;
    sf::Vector2f currentOrientation = GetDirectionFacing(m_player.GetPlayerPos(), m_enemy.GetPosition());
    targetOrientation = wanderOrientation + AngleToVector(m_enemy.GetRotation());
    sf::Vector2f target = m_enemy.GetPosition() + wanderOffset * AngleToVector(m_enemy.GetRotation());

    target += wanderRadius * targetOrientation;

    //steeringOutput = face.getSteering(target);

    steeringOutput.linear = maxAcceleration * AngleToVector(m_enemy.GetRotation());

    // Angular
    float angle = atan2(direction.y, direction.x) * 180 / 3.14159265;
    steeringOutput.angular = angle;
    

    return steeringOutput;
}


