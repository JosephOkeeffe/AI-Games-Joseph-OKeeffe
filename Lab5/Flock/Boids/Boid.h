#include "Pvector.h"
#include <vector>
#include <stdlib.h>
#include <iostream>
#include "SFML/Graphics.hpp"

using namespace std;

#ifndef BOID_H_
#define BOID_H_

class Boid
{
public:
	bool predator;
	bool isLeader = false;
	float orientation; //Direction facing in radians. 0 is up.
	Pvector location;
	Pvector velocity;
	Pvector acceleration;
	float maxSpeed;
	float maxForce;
	int neighbourDistance = 75;

	sf::Transform transform;


	Boid() {}
	Boid(float x, float y)
	{
		acceleration = Pvector(0, 0);
		velocity = Pvector(rand()%3-2, rand()%3-2); // Allows for range of -2 -> 2
		location = Pvector(x, y);
		maxSpeed = 2.5;
		maxForce = 0.5;
	}
	Boid(float x, float y, bool predCheck) 
	{
		predator = predCheck;
		if (predCheck == true) 
		{
			maxSpeed = 7.5;
			maxForce = 0.5;
			velocity = Pvector(rand()%3-1, rand()%3-1);
		} else 
		{
			maxSpeed = 1.0;
			maxForce = 0.2;
			velocity = Pvector(rand()%3-2, rand()%3-2); // Allows for range of -2 -> 2
		}
		acceleration = Pvector(0, 0);
		location = Pvector(x, y);
	}


	sf::Vector2f getVectorFromAngle(float a);
	void accelerate(int power);
	void steer(int direction);
	float getNewOrientation();
	void applyForce(Pvector& force);
	// Three Laws that boids follow
	Pvector Separation(vector<Boid>& Boids);
	Pvector Alignment(vector<Boid>& Boids);
	Pvector Cohesion(vector<Boid>& Boids);
	//Functions involving SFML and visualisation linking
	Pvector seek(Pvector& v);
	void run(vector <Boid>& v);
	void update(string formation);
	void flock(vector <Boid>& v);
	void WrapAround();
	float angle(Pvector& v);
	void swarm(vector <Boid>& v);

	sf::Clock clock;
};

#endif