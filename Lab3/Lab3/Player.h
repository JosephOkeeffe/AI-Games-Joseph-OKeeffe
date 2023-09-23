#pragma once
#include <SFML/Graphics.hpp> 
#include<SFML/System.hpp>

class Player
{
public:

	Player();
	void Init();
	void Render(sf::RenderWindow& window);
	void Update(sf::RenderWindow& window);

	sf::Vector2f GetPlayerPos();


	sf::Sprite playerSprite;
	sf::Texture playerTexture;


private:

	void Move();
	void ChangeVelocity();
	void WrapAround(sf::RenderWindow& window);
	sf::Vector2f Normalise(sf::Vector2f vector);
	float VectorLength(sf::Vector2f vector);

	void CalculateVisionCone();

	float moveSpeed = 0.5f;
	const float MAX_SPEED = 3;
	sf::Vector2f startPosition {100, 100};
	sf::Vector2f velocity = { 0, 0 };
	float acceleration = 0.1f;

	float scale = 0.5f;
	float rotation;
	float rotationSpeed = 1.0f;

	//sf::VertexArray visionConeColor(sf::Triangles, 3);
	sf::VertexArray visionCone{ sf::Triangles, 3 };
	sf::VertexArray line{ sf::Lines, 2 };
	
	sf::Vector2f coneVertex1;
	sf::Vector2f coneVertex2;
	sf::Vector2f coneVertex3;
	sf::Color startColor = sf::Color(255, 255, 0, 220);
	sf::Color endColor = sf::Color(255, 255, 204, 45);

	float coneLength = 200;
	float angleOffset = 200;

};

