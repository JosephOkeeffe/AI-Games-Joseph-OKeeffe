#include "Tile.h"
#include <iostream>
#include "Global.h"

void Tile::Init(sf::Vector2f& position, sf::Font& font)
{
	tile.setFillColor(sf::Color::Transparent);
	tile.setSize(sf::Vector2f(Global::CELL_SIZE, Global::CELL_SIZE));
	tile.setOutlineThickness(1);
	tile.setOutlineColor(sf::Color(0,0,0, 255));
	tile.setPosition(position);

	text.setFont(font);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(Global::CELL_SIZE / 2.5);
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
		if (!drawingLines && drawCost || drawIntegrationField)
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
		
	}
	else if (isGoalTile)
	{
		tile.setFillColor(sf::Color::Red);
	}
	else
	{
		tile.setFillColor(sf::Color::Transparent);
	}
	UpdateTextOnScreen();
}

void Tile::UpdateTextOnScreen()
{
	if (drawCost)
	{
		text.setString(std::to_string(cost));
	}
	else if (drawIntegrationField)
	{
		text.setString(std::to_string(integrationField));
	}
}

void Tile::TurnOnCost()
{
	text.setString(std::to_string(cost));
	drawCost = true;
	drawIntegrationField = false;
	drawingLines = false;
}

void Tile::TurnOnIntegration()
{
	text.setString(std::to_string(integrationField));
	drawCost = false;
	drawIntegrationField = true;
	drawingLines = false;


}

void Tile::TurnOnLines()
{
	drawCost = false;
	drawIntegrationField = false;
	drawingLines = true;

}


