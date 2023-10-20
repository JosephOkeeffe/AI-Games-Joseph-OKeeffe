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


	sf::RectangleShape tile;
	int gridCellWidth = 26;
	int cost;
	int integrationField; // cost + distance
	sf::Vector2f floField;

	bool isStartTile = false;
	bool isGoalTile = false;
	bool isObstacleTile = false;

	//sf::Text costDisplay; // show costs on screen
};

