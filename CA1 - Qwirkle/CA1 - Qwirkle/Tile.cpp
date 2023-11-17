#include "Tile.h"
#include <iostream>

Tile::Tile()
{
}

void Tile::Init(sf::Vector2f& position)
{

	tile.setFillColor(sf::Color::Black);
	tile.setSize(sf::Vector2f(Global::CELL_SIZE, Global::CELL_SIZE));
	tile.setOutlineThickness(1);
	tile.setOutlineColor(sf::Color(255,255,255, 255));
	tile.setPosition(position);

	currentPiece = NONE;
	shape.setOrigin(shape.getRadius(), shape.getRadius());
	shape.setPosition(position.x + Global::CELL_SIZE / 2, position.y + Global::CELL_SIZE / 2);

	CheckPiece();
}

void Tile::Render(sf::RenderWindow& window)
{
	window.draw(tile);
	window.draw(shape);
}

void Tile::CheckPiece()
{
	SetShape();
	SetColor();
}

void Tile::SetShape()
{
	int mod = currentPiece % 6;

	if (currentPiece >= 0 && currentPiece < 6)
	{
		shape.rotate(45);
	}
	else
	{
		shape.rotate(0);
	}

	if (currentPiece >= 0 && currentPiece < 6) 
	{
		//shape.rotate(45);
		shapeSides = 4;  // Square
	}
	else if (currentPiece >= 6 && currentPiece < 12) 
	{
		shapeSides = 100;  // Circle
	}
	else if (currentPiece >= 12 && currentPiece < 18) 
	{
		shapeSides = 3;  // Triangle
	}
	else if (currentPiece >= 18 && currentPiece < 24) 
	{
		shapeSides = 4;  // Diamond
	}
	else if (currentPiece >= 24 && currentPiece < 30) 
	{
		shapeSides = 6;  // Hexagon
	}
	else if (currentPiece >= 30 && currentPiece < 36) 
	{
		shapeSides = 8;  // Octagon
	}

	shape.setPointCount(shapeSides);
}

void Tile::SetColor()
{
	switch (currentPiece % 6)
	{
	case 0: shapeColor = sf::Color::Red; break;
	case 1: shapeColor = sf::Color::Green; break;
	case 2: shapeColor = sf::Color::Blue; break;
	case 3: shapeColor = sf::Color::Yellow; break;
	case 4: shapeColor = sf::Color::Magenta; break;
	case 5: shapeColor = sf::Color::White; break;
	default: shapeColor = sf::Color::Transparent; break;
	}

	shape.setFillColor(shapeColor);
}

void Tile::SetRandomTile()
{
	
	int random = rand() % 36;
	currentPiece = static_cast<Piece>(random);
	CheckPiece();
	
}

