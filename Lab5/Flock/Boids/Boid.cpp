// This file defines the boid class. This includes the attributes found in
// boids -- speed, location on the board, acceleration, etc.
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include "SFML/Graphics.hpp"
#include "Boid.h"

// Global Variables for borders()
// desktopTemp gets screen resolution of PC running the program
sf::VideoMode desktopTemp = sf::VideoMode::getDesktopMode(); 
const int window_height = desktopTemp.height;
const int window_width = desktopTemp.width;

#define w_height window_height
#define w_width window_width
#define PI 3.141592635

using namespace std;

// =============================================== //
// ======== Boid Functions from Boid.h =========== //
// =============================================== //

void Boid::accelerate(int power) {
	if (power == 1) 
	{//Accelerate.
		//		acceleration.mulScalar(1.5);
				// Update velocity
		Pvector newVelocity = velocity;
		// Limit speed
		if (newVelocity.magnitude() <= maxSpeed)
		{
			velocity.mulScalar(1.5);
		}

	}
	else 
	{//Decelerate.
		//		acceleration.mulScalar(.5);
		velocity.mulScalar(.75);
	}
	std::cout << "Acceleration = " << acceleration.magnitude() << std::endl;
	std::cout << "Velocity = " << velocity.magnitude() << std::endl;

}

void Boid::steer(int direction) {

	if (direction == 1) 
	{//Turn right.
		orientation += 6.0;// / 180.0 * PI;
	}
	else 
	{//Turn left.
		orientation -= 6.0;// / 180.0 * PI;
	}
	//	velocity.set(cos(orientation * (PI / 180)), sin(orientation * (PI / 180)));
	sf::Vector2f tempVelocity = getVectorFromAngle(orientation);
	float magnitude = velocity.magnitude();
	velocity.set(tempVelocity.x, tempVelocity.y);
	velocity.mulScalar(magnitude);	// We need to ensure we reset the size of the vector as it was before we computed the new unit vector velocity.
}

float Boid::getNewOrientation() {

	if (velocity.magnitude() > 0) 
	{
		float rads = atan2(velocity.y, velocity.x); // atan2 returns radians
		float newAngle = rads * 180 / PI;  
		// Convert to degrees
		return newAngle + 90;	// Becasue SFML orientation has 0 degrees pointing up, while normal trigonometry has 0 along the x axis.
	}
	else
	{
		return orientation;
	}
}

sf::Vector2f Boid::getVectorFromAngle(float a)
{
	float rads = (a - 90) / 180.0 * PI; // Convert to radians. 
	// First subtact 90 to move back to normal cordinate space (not SFML space) for COS and SIN to give us the corect values.
	sf::Vector2f newVector = sf::Vector2f(cos(rads), sin(rads));
	return newVector;
}

// Adds force Pvector to current force Pvector
void Boid::applyForce(Pvector& force)
{
	acceleration.addVector(force);
}

// Function that checks and modifies the distance
// of a boid if it breaks the law of separation.
Pvector Boid::Separation(vector<Boid>& boids)
{
	// If the boid we're looking at is a predator, do not run the separation
	// algorithm
	
	// Distance of field of vision for separation between boids
	// For simplicity we are not implementing a cone of vision here.
	float desiredseparation = 20;

	Pvector steer(0, 0);
	int count = 0;
	// For every boid in the system, check if it's too close
	for (int i = 0; i < boids.size(); i++)
	{
		// Calculate distance from current boid to boid we're looking at
		float d = location.distance(boids[i].location);
		if (d < neighbourDistance) //Only nearest neighbours
		{
			// If this is a fellow boid and it's too close, move away from it
			if ((d > 0) && (d < desiredseparation))
			{
				Pvector diff(0, 0);
				diff = diff.subTwoVector(location, boids[i].location);
				diff.normalize();
				diff.divScalar(d);      // Weight by distance
				steer.addVector(diff);
				count++;
			}
			// If current boid is a predator and the boid we're looking at is also
			// a predator, then separate only slightly 
			if ((d > 0) && (d < desiredseparation) && predator == true && boids[i].predator == true)
			{
				Pvector pred2pred(0, 0);
				pred2pred = pred2pred.subTwoVector(location, boids[i].location);
				pred2pred.normalize();
				pred2pred.divScalar(d);
				steer.addVector(pred2pred);
				count++;
			}
			// If current boid is not a predator, but the boid we're looking at is
			// a predator, then create a large separation Pvector
			else if ((d > 0) && (d < desiredseparation + 70) && boids[i].predator == true)
			{
				Pvector pred(0, 0);
				pred = pred.subTwoVector(location, boids[i].location);
				pred.mulScalar(900);
				steer.addVector(pred);
				count++;
			}
		}
	}
	// Adds average difference of location to acceleration
	if (count > 0)
		steer.divScalar((float)count);
	if (steer.magnitude() > 0) 
	{
		// Steering = Desired - Velocity
		steer.normalize();
		steer.mulScalar(maxSpeed);
		steer.subVector(velocity);
		steer.limit(maxForce);
	}
	return steer;
}

// Alignment calculates the average velocity in the field of view and 
// manipulates the velocity of the Boid passed as parameter to adjust to that
// of nearby boids.
Pvector Boid::Alignment(vector<Boid>& Boids)
{
	// If the boid we're looking at is a predator, do not run the alignment
	// algorithm
	//if (predator == true)
	//	return Pvector(0,0);
//	float neighbordist = 50;

	Pvector sum(0, 0);	
	int count = 0;
	for (int i = 0; i < Boids.size(); i++)
	{
		float d = location.distance(Boids[i].location);
		if ((d > 0) && (d < neighbourDistance)) // 0 < d < 50. Only nearest neighbours
		{
			sum.addVector(Boids[i].velocity);
			count++;
		}
	}
	// If there are boids close enough for alignment...
	if (count > 0)
	{
		sum.divScalar((float)count);// Divide sum by the number of close boids (average of velocity)
		sum.normalize();	   		// Turn sum into a unit vector, and
		sum.mulScalar(maxSpeed);    // Multiply by maxSpeed
		// Steer = Desired - Velocity
		Pvector steer; 
		steer = steer.subTwoVector(sum, velocity); //sum = desired(average)  
		steer.limit(maxForce);
		return steer;
	} else {
		Pvector temp(0, 0);
		return temp;
	}
}

// Cohesion finds the average location of nearby boids and manipulates the 
// steering force to move in that direction.
Pvector Boid::Cohesion(vector<Boid>& Boids)
{
	// If the boid we're looking at is a predator, do not run the cohesion
	// algorithm
	//if (predator == true)
	//	return Pvector(0,0);

//	float neighbordist = 50;

	Pvector sum(0, 0);	
	int count = 0;
	for (int i = 0; i < Boids.size(); i++)
	{
		float d = location.distance(Boids[i].location);
		if ((d > 0) && (d < neighbourDistance))
		{
			sum.addVector(Boids[i].location);
			count++;
		}
	}
	if (count > 0)
	{
		sum.divScalar(count);
		return seek(sum);
	} else {
		Pvector temp(0,0);
		return temp;
	}
}

// Seek function limits the maxSpeed, finds necessary steering force and
// normalizes the vectors.
Pvector Boid::seek(Pvector& v)
{
	Pvector desired;
	desired.subVector(v);  // A vector pointing from the location to the target
	// Normalize desired and scale to maximum speed
	desired.normalize();
	desired.mulScalar(maxSpeed);
	// Steering = Desired minus Velocity
	acceleration.subTwoVector(desired, velocity);
	acceleration.limit(maxForce);  // Limit to maximum steering force
	return acceleration;
}

//Update modifies velocity, location, and resets acceleration with values that
//are given by the three laws.
void Boid::update(string formation)
{
	if (formation == "custom")	// Need to treat the leader differently
	{
		if (isLeader)
		{
			std::cout << "Leader velocity = " << velocity.x << ", " << velocity.y << std::endl;
			// Update velocity
			location.addVector(velocity);

		}
		else
		{
			// Update velocity
			velocity.addVector(acceleration);
			// Limit speed
			velocity.limit(maxSpeed * 3);	// Without this higher speedup, the formation members were going too slow to cath the leader and get into formation.
			location.addVector(velocity);
			// Reset accelertion to 0 each cycle
			acceleration.mulScalar(0);
		}
	}
	else
	{
		//To make the slow down not as abrupt
		acceleration.mulScalar(.4);
		// Update velocity
		velocity.addVector(acceleration);
		// Limit speed
		velocity.limit(maxSpeed);
		location.addVector(velocity);
		// Reset accelertion to 0 each cycle
		acceleration.mulScalar(0);
	}
}

//Run runs flock on the flock of boids for each boid.
//Which applies the three rules, modifies accordingly, updates data, checks is data is
//out of range, fixes that for SFML, and renders it on the window.
void Boid::run(vector <Boid>& v)
{
	flock(v);
	update("flock");
	WrapAround();
}

//Applies all three laws for the flock of boids and modifies to keep them from
//breaking the laws.
void Boid::flock(vector<Boid>& v) 
{
	// optimisation
	
	Pvector sep,ali,coh;
	if (clock.getElapsedTime().asSeconds() > 0.05)
	{
		sep = Separation(v);
		ali = Alignment(v);
		coh = Cohesion(v);

		// Arbitrarily weight these forces
		sep.mulScalar(1.5);
		ali.mulScalar(1.0); // Might need to alter weights for different characteristics
		coh.mulScalar(1.0);
		// Add the force vectors to acceleration
		applyForce(sep);
		applyForce(ali);
		applyForce(coh);
		clock.restart();
	}
	

}

// Checks if boids go out of the window and if so, wraps them around to the other side.
void Boid::WrapAround()
{
	if (location.x < 0) location.x = w_width; 
	if (location.y < 0) location.y += w_height;
	if (location.x > w_width) location.x = 0;
	if (location.y > w_height) location.y = 0;
}

// Calculates the angle for the velocity of a boid which allows the visual 
// image to rotate in the direction that it is going in.
float Boid::angle(Pvector& v)
{
	// From the definition of the dot product
	float angle = (float)(atan2(v.x, -v.y) * 180 / PI);
	return angle;
}

void Boid::swarm(vector <Boid>& v)
{
	float A = 40;  // Attraction
	float B = 4000;  // Repulsion
	float N = 1;  // Attenuation values
	float M = 2;
	Pvector sum(0,0);

	for (int i = 0; i < v.size(); i++)
	{
		if (&v[i] != this)
		{
			Pvector	R(0, 0);
			
			R = R.subTwoVector(location, v[i].location);
			float D = R.magnitude();

			if (D > 0)
			{
				float U = (-A / powf(D, N)) + (B / powf(D, M));
				R.normalize();
				R = R * U;
				sum.addVector(R);
			}
		
		}
	}
	applyForce(sum);
	update("swarm");
	WrapAround();
}
