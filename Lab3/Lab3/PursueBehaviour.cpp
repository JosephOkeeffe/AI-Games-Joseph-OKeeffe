#include "PursueBehaviour.h"

PursueBehaviour::PursueBehaviour(Enemy& enemy, Player& player) : m_enemy(enemy), m_player(player)
{
}

SteeringOutput PursueBehaviour::GetSteering()
{
	return SteeringOutput();
}

//direction = target.position - my.position
//distance = direction.length
//speed = my.velocity.length
//if speed <= distance / maxTimePrediction:
//timePrediction = maxTimeprediction
//else:
//timePrediction = distance / speed
//newtarget.position = target.position + target.velocity * timePrediction
//seek(me, newTarget)
//
//or if we encapsulate this code in a Pursue function :
//return seek(me, newTarget)

