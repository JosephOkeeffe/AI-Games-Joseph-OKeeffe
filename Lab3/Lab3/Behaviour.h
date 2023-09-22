#pragma once
#include <SFML/Graphics.hpp>
#include "SteeringOutput.h"


class Behaviour 
{
public:

    virtual SteeringOutput GetSteering() { return SteeringOutput(); }

    sf::Vector2f Normalise(sf::Vector2f vector);
    float VectorLength(sf::Vector2f vector);

};