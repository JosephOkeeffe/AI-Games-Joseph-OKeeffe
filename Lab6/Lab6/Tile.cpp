#include "Tile.h"
#include <iostream>

void Tile::Init(sf::Vector2f& position)
{
	tile.setFillColor(sf::Color::Transparent);
	tile.setSize(sf::Vector2f(gridCellWidth, gridCellWidth));
	tile.setOutlineThickness(0.5f);
	tile.setOutlineColor(sf::Color(255,255,255, 100));
	tile.setPosition(position);
}

void Tile::Render(sf::RenderWindow& window)
{
	window.draw(tile);
}

void Tile::SetStart()
{
	tile.setFillColor(sf::Color::Green);

	isStartTile = true;
	isObstacleTile = false;
	isGoalTile = false;
}

void Tile::SetObstacle()
{
	tile.setFillColor(sf::Color::White);

	isStartTile = false;
	isObstacleTile = true;
	isGoalTile = false;
}

void Tile::SetGoal()
{
	tile.setFillColor(sf::Color::Red);
	isStartTile = false;
	isObstacleTile = false;
	isGoalTile = true;
}


