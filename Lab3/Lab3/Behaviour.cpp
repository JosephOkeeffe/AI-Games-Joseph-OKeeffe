#include "Behaviour.h"

sf::Vector2f Behaviour::Normalise(sf::Vector2f vector)
{
	sf::Vector2f normalised = {0.0f, 0.0f};
	float length = VectorLength(vector);

    if (length == 0.0f) 
        return vector;
    
    normalised.x = vector.x / length;
    normalised.y = vector.y / length;
    
    return normalised;
}

float Behaviour::VectorLength(sf::Vector2f vector)
{
	float vLength = std::sqrt(vector.x * vector.x + vector.y * vector.y);
	return vLength;
}

sf::Vector2f Behaviour::GetDirectionFacing(sf::Vector2f targetPos, sf::Vector2f currentPos)
{
    return targetPos - currentPos;
}

sf::Vector2f Behaviour::AngleToVector(float angle)
{
    sf::Vector2f vector;
    vector.x = cos(angle * (3.14159265 / 180));
    vector.y = sin(angle * (3.14159265 / 180));
    return vector;
}

