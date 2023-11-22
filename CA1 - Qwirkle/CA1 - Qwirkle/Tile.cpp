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

	shape.setRadius(tile.getSize().x / 2.5);
	shape.setOrigin(shape.getRadius(), shape.getRadius());
	shape.setPosition(position.x + Global::CELL_SIZE / 2, position.y + Global::CELL_SIZE / 2);
	shape.setOutlineThickness(2);
	shape.setOutlineColor(sf::Color::Black);

	tileShape = "None";
	tileColor = "None";
	

	CheckPiece();
}

void Tile::Render(sf::RenderWindow& window)
{
	window.draw(tile);
	if (!isUsed)
	{
		window.draw(shape);
	}
}

void Tile::CheckPiece()
{
	SetShape();
	SetColor();
	tileName = tileShape + " " + tileColor;
	
}

void Tile::SetShape()
{
 	CheckCurrentPieceSize(currentPiece);

	if (currentPiece >= 0 && currentPiece < 6)
	{
		shape.setRotation(45);
	}
	else
	{
		shape.setRotation(0);
	}

	if (currentPiece >= 0 && currentPiece < 6) 
	{
		tileShape = "Square";
		shapeSides = 4;  // Square
	}
	else if (currentPiece >= 6 && currentPiece < 12) 
	{
		tileShape = "Circle";
		shapeSides = 100;  // Circle
	}
	else if (currentPiece >= 12 && currentPiece < 18) 
	{
		tileShape = "Triangle";
		shapeSides = 3;  // Triangle
	}
	else if (currentPiece >= 18 && currentPiece < 24) 
	{
		tileShape = "Diamond";
		shapeSides = 4;  // Diamond
	}
	else if (currentPiece >= 24 && currentPiece < 30) 
	{
		tileShape = "Hexagon";
		shapeSides = 6;  // Hexagon
	}
	else if (currentPiece >= 30 && currentPiece < 36) 
	{
		tileShape = "Octagon";
		shapeSides = 8;  // Octagon
	}
	else
	{
		tileShape = "Used";
		shapeSides = 0;
	}

	shape.setPointCount(shapeSides);
}

void Tile::SetColor()
{
	switch (currentPiece % 6)
	{
	case 0: shapeColor = sf::Color::Red; 
		tileColor = "Red";
		break;
	case 1: shapeColor = sf::Color::Green; 
		tileColor = "Green";
		break;
	case 2: shapeColor = sf::Color::Blue;
		tileColor = "Blue";
		break;
	case 3: shapeColor = sf::Color::Yellow;
		tileColor = "Yellow";
		break;
	case 4: shapeColor = sf::Color::Magenta; 
		tileColor = "Pink";
		break;
	case 5: shapeColor = sf::Color::White; 
		tileColor = "White";
		break;
	default: shapeColor = sf::Color::Transparent; 
		tileColor = "None";
		break;
	}

	shape.setFillColor(shapeColor);
}

void Tile::SetPiece(int piece)
{
	currentPiece = static_cast<Piece>(piece);
	CheckCurrentPieceSize(currentPiece);
	isPlaced = true;
	CheckPiece();
}

void Tile::SelectTile()
{
	tile.setFillColor(sf::Color(255, 99, 71, 200));
	isSelected = true;
}

void Tile::DeselectTile()
{
	tile.setFillColor(sf::Color::Black);
	isSelected = false;
}

Piece Tile::GetCurrentPiece()
{
	return currentPiece;
}

void Tile::SetUsed()
{
	isUsed = true;
	CheckPiece();
}

bool Tile::GetUsed()
{
	return isUsed;
}

void Tile::CheckCurrentPieceSize(int piece)
{
	if (piece >= 36)
	{
		int tooBig = 0;
		tooBig = piece;
		tooBig -= 36;
		currentPiece = static_cast<Piece>(tooBig);
		CheckCurrentPieceSize(currentPiece);
	}
}



