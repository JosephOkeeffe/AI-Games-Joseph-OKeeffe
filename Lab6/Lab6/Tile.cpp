#include "Tile.h"
#include <iostream>
#include "Global.h"

void Tile::Init(sf::Vector2f& position)
{
	tile.setFillColor(sf::Color::Transparent);
	tile.setSize(sf::Vector2f(Global::CELL_SIZE, Global::CELL_SIZE));
	tile.setOutlineThickness(0.5f);
	tile.setOutlineColor(sf::Color(255,255,255, 100));
	tile.setPosition(position);

	if (!font.loadFromFile("./ASSETS/FONTS/ariblk.ttf")) {
		std::cout << "ajnd \n";
	}
	text.setFont(font);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(Global::CELL_SIZE / 4);
	text.setString(std::to_string(cost));
	text.setPosition(tile.getPosition().x + (tile.getSize().x / 4), tile.getPosition().y + (tile.getSize().y / 2));
	text.setOutlineThickness(1);
	text.setOutlineColor(sf::Color::Black);
}

void Tile::Render(sf::RenderWindow& window)
{
	window.draw(tile);
	window.draw(text);
}

void Tile::SetStart()
{
	isStartTile = true;
	isObstacleTile = false;
	isGoalTile = false;
	CheckStatus();
}

void Tile::SetObstacle()
{
	isStartTile = false;
	isObstacleTile = true;
	isGoalTile = false;
	CheckStatus();
}

void Tile::Reset()
{
	isStartTile = false;
	isObstacleTile = false;
	isGoalTile = false;
	CheckStatus();
}

void Tile::SetGoal()
{
	isStartTile = false;
	isObstacleTile = false;
	isGoalTile = true;
	CheckStatus();
}

void Tile::CheckStatus()
{
	if (isStartTile)
	{
		tile.setFillColor(sf::Color::Green);
	}
	else if (isObstacleTile)
	{
		cost = 9999;
		tile.setFillColor(sf::Color::White);
		UpdateCost();
	}
	else if (isGoalTile)
	{
		tile.setFillColor(sf::Color::Red);
	}
	else
	{
		tile.setFillColor(sf::Color::Transparent);
	}
}

void Tile::UpdateCost()
{
	text.setString(std::to_string(cost));
}

