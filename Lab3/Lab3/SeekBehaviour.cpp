#include "SeekBehaviour.h"
#include "SteeringOutput.h"
#include <iostream>

SeekBehaviour::SeekBehaviour(Enemy& enemy,Player& player) : m_enemy(enemy), m_player(player)
{
}

SteeringOutput SeekBehaviour::GetSteering()
{
    SteeringOutput steeringOutput;
    steeringOutput.linear = m_player.GetPlayerPos() - m_enemy.GetPosition();
    
    Normalise(steeringOutput.linear);

    steeringOutput.linear *= maxAcceleration;

    steeringOutput.angular = { 0,0 };

    std::cout << "Linear" << steeringOutput.linear.x << "\n";

    return steeringOutput;
}
