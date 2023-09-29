#include "PursueBehaviour.h"

PursueBehaviour::PursueBehaviour(Enemy& enemy, Player& player) : m_enemy(enemy), m_player(player)
{
}

SteeringOutput PursueBehaviour::GetSteering()
{
    SteeringOutput steeringOutput;

    sf::Vector2f direction = m_player.GetPlayerPos() - m_enemy.GetPosition();
    float distance = VectorLength(direction);
    float speed = VectorLength(m_enemy.GetVelocity());

    if (speed <= distance / maxTimePrediction)
    {
        timePrediction = maxTimePrediction;
    }
    else
    {
        timePrediction = distance / speed;
    }

    sf::Vector2f tempPos = m_player.GetPlayerPos();
    sf::Vector2f tempVel = m_player.GetVelocity();
    sf::Vector2f newTargetPosition = m_player.GetPlayerPos() + m_player.GetVelocity() * timePrediction;

    Player newTarget;
    newTarget.SetPosition(newTargetPosition);

    SeekBehaviour seek(m_enemy, newTarget);
   
    steeringOutput = seek.GetSteering();

  

    return steeringOutput;

}


