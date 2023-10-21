#pragma once
#include <SFML/Graphics.hpp>
class Tile
{
public:

	void Init(sf::Vector2f& position);
	void Render(sf::RenderWindow& window);
	void SetStart();
	void SetGoal();
	void SetObstacle();
	void Reset();
	void CheckStatus();
	void UpdateCost();

	sf::RectangleShape tile;
	int cost = 0;
	int integrationField; // cost + distance
	sf::Vector2f floField;

	bool isStartTile = false;
	bool isGoalTile = false;
	bool isObstacleTile = false;

	sf::Font font;
	sf::Text text;

};

