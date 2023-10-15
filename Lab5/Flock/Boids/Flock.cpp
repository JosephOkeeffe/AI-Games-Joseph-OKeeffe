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
	int flockSize = flock.size();
	int widthSpacing = 20;  // Horizontal spacing between NPCs
	int heightSpacing = 20;  // Vertical spacing between NPCs
	int formationSides = 4;
	int boidsPerLine = 11;
	int downCount = 1;
	int acrossCount = 0;
	int acrossOffset = 105;


	for (int i = 0; i < flockSize; i++)
	{
		Pvector sum(0, 0);
		Pvector targetSlot(0, 0);
		Boid target = flock[leader]; // Our designated leader
		int closeEnough = 10;

		if (i == leader) 
		{  // Deal with our leader here
			flock[i].update("custom");
			flock[i].WrapAround();
		}
		else 
		{
			// getting the leader boids position
			targetSlot = target.location;
			targetSlot.x -= acrossOffset - (widthSpacing * acrossCount);
			targetSlot.y += heightSpacing * downCount;
			acrossCount++;

			if (acrossCount % 11 == 0)
			{
				acrossCount = 0;
			}	
			if (i % boidsPerLine == 0)
			{
				downCount++;
			}
			if (downCount / 11 == 1)
			{
				downCount = 1;
			}

			Pvector sub = sub.subTwoVector(targetSlot, flock[i].location);
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
