#pragma once
#include <SFML/Graphics.hpp>
class Tile
{
public:

	void Init(sf::Vector2f& position, sf::Font& font);
	void Render(sf::RenderWindow& window);
	void SetStart();
	void SetGoal();
	void SetObstacle();
	void Reset();
	void CheckStatus();
	void UpdateTextOnScreen();

	void TurnOnCost();
	void TurnOnIntegration();
	void TurnOnLines();

	sf::RectangleShape tile;
	int cost = 0;
	int integrationField; // cost + distance
	sf::Vector2f floField;

	bool isStartTile = false;
	bool isGoalTile = false;
	bool isObstacleTile = false;
	bool isPathTile = false;

	bool drawingLines = false;
	bool drawCost = false;
	bool drawIntegrationField = false;

	sf::Font font;
	sf::Text text;

	sf::Vertex line[2];

};

