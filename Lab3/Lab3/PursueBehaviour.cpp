#include "PursueBehaviour.h"

PursueBehaviour::PursueBehaviour(Enemy& enemy, Player& player) : m_enemy(enemy), m_player(player)
{
}

SteeringOutput PursueBehaviour::GetSteering()
{
	return SteeringOutput();
}
