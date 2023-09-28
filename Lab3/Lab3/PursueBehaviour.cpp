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

    sf::Vector2f newTargetPosition = m_player.GetPlayerPos() + m_player.GetVelocity() * timePrediction;

    // Linear
    steeringOutput.linear = newTargetPosition - m_enemy.GetPosition();
    Normalise(steeringOutput.linear);
    steeringOutput.linear *= maxAcceleration;

    direction = steeringOutput.linear;
    float angle = atan2(direction.y, direction.x) * 180 / 3.14159265;
    steeringOutput.angular = angle;

    return steeringOutput;

}


