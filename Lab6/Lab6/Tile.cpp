#include "Tile.h"
#include <iostream>
#include "Global.h"

void Tile::Init(sf::Vector2f& position, sf::Font& font)
{
	tile.setFillColor(sf::Color::Blue);
	tile.setSize(sf::Vector2f(Global::CELL_SIZE, Global::CELL_SIZE));
	tile.setOutlineThickness(0.5f);
	tile.setOutlineColor(sf::Color(0,0,0, 255));
	tile.setPosition(position);

	
	text.setFont(font);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(Global::CELL_SIZE / 2);
	text.setString(std::to_string(cost));
	text.setPosition(tile.getPosition().x + (tile.getSize().x / 4.5), tile.getPosition().y + (tile.getSize().y / 3));
	text.setOutlineThickness(1);
	text.setOutlineColor(sf::Color::Black);
}

void Tile::Render(sf::RenderWindow& window)
{
	window.draw(tile);
	
	if (!isObstacleTile)
	{
		if (drawCost)
		{
			window.draw(text);
		}
		else
		{
			window.draw(line, 2, sf::Lines);
		}
	}

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
		cost = 99;
		tile.setFillColor(sf::Color::Black);
		UpdateCost();
	}
	else if (isGoalTile)
	{
		tile.setFillColor(sf::Color::Red);
	}
}

void Tile::UpdateCost()
{
	text.setString(std::to_string(cost));
}


