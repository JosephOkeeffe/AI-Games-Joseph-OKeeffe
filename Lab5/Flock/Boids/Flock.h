#include <iostream>
#include <vector>
#include "Boid.h"

using namespace std;

#ifndef FLOCK_H_
#define FLOCK_H_

class Flock
{
private:

public:
	//Public Variables
	vector<Boid> flock;

	//Constructors
	Flock() {}
	//Accessor functions
	int getSize();
	Boid* getBoid(int i);
	//Mutator Functions
	void addBoid(Boid b);
	void flocking();
	void swarming();
	void CustomFormation(int leader);
	void LineFormation(int leader);
	sf::Transform transform;


};

#endif