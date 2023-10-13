#include "Boid.h"
#include "Flock.h"

using namespace std;

// =============================================== //
// ======== Flock Functions from Flock.h ========= //
// =============================================== //

int Flock::getSize()
{
	return flock.size();
}

Boid* Flock::getBoid(int i)
{
	return &flock[i];
}

void Flock::addBoid(Boid b)
{
	flock.push_back(b);
}

// Runs the run function for every boid in the flock checking against the flock
// itself. Which in turn applies all the rules to the flock.
void Flock::flocking() 
{
	for (int i = 0; i < flock.size(); i++)
	{
		flock[i].run(flock);
	}
}
// Runs the swarm function for every boid in the flock checking against the flock
// itself. Which in turn applies all the rules to the flock.
void Flock::swarming()
{
	for (int i = 0; i < flock.size(); i++)
	{
		flock[i].swarm(flock);
	}
}

void Flock::CustomFormation(int leader)
{
	float pi = 3.141592653589793;
	int fSize = flock.size();
	for (int i = 0; i < fSize; i++)
	{
		Pvector	sub(0, 0);
		Pvector sum(0, 0);
		int npcRadius = 10;
		int closeEnough = 10;
		float angleAroundCircle = 0.0;
		Pvector targetSlot(0, 0);
		Boid target = flock[leader]; // Our designated leader
		


		if (i == leader) 
		{		//Deal with our leader here
			// Any changes to velocity will be done in the steering function
			flock[i].update("custom");
			flock[i].WrapAround();
		}
		else 
		{    //Find our position in the circle
			angleAroundCircle = (float)i / (fSize - 1);
			angleAroundCircle = angleAroundCircle * pi * 2;
			float radius = npcRadius / sin(pi / (fSize));

			targetSlot = target.location;
			targetSlot.x = targetSlot.x - radius * cos(angleAroundCircle);
			targetSlot.y = targetSlot.y - radius * sin(angleAroundCircle);
			sub = sub.subTwoVector(targetSlot, flock[i].location);
			float D = sub.magnitude();
			if (D > closeEnough)	// Are we close enough to our slot position, if so just match the leader's velocity.
			{
				//				std::cout << "NOT close enough" << std::endl;
				sum = sub;
				sum.normalize();
				sum.mulScalar(flock[i].maxSpeed);
				flock[i].applyForce(sum);
				flock[i].update("custom");
				flock[i].WrapAround();
			}
			else
			{
				flock[i].velocity = flock[leader].velocity; //Match the leader's velocity if we are close enough
				flock[i].WrapAround();
				//				std::cout << "Close enough" << flock[fLeader].velocity.magnitude() << std::endl;
			}
		}
	}
}

void Flock::LineFormation(int leader)
{
	float pi = 3.141592653589793;
	int fSize = flock.size();
	int npcRadius = 10;
	int closeEnough = 10;

	// Find the leader's position in the formation
	Pvector leaderSlot(0, 0);
	leaderSlot = flock[leader].location;

	for (int i = 0; i < fSize; i++) 
	{
		Pvector sub(0, 0);
		Pvector sum(0, 0);
		float angleAroundCircle = 0.0;
		Pvector targetSlot(0, 0);
		sf::Vector2f leaderPos;
		leaderPos.x = flock[leader].location.x;
		leaderPos.y = flock[leader].location.x;


		if (i == leader) 
		{
			// Deal with our leader here
			// Any changes to velocity will be done in the steering function
			flock[i].update("custom");
			flock[i].WrapAround();
		}
		else 
		{
			// Find the position in the line behind the leader
			angleAroundCircle = (float)i / (fSize - 1);
			angleAroundCircle = angleAroundCircle * pi * 2;
			float radius = npcRadius * (i - leader);

			targetSlot.x = leaderSlot.x - radius;
			targetSlot.y = leaderSlot.y;

			sub = sub.subTwoVector(targetSlot, flock[i].location);
			float D = sub.magnitude();

			if (D > closeEnough) 
			{
				sum = sub;
				sum.normalize();
				sum.mulScalar(flock[i].maxSpeed);
				flock[i].applyForce(sum);
				flock[i].update("custom");
				
				flock[i].WrapAround();
			}
			else
			{
				flock[i].velocity = flock[leader].velocity;
				flock[i].WrapAround();
			}
			flock[i].transform.translate(leaderPos);
			flock[i].transform.rotate(15, leaderPos);
		}

		//transform.translate(leaderPos);


	}
}
