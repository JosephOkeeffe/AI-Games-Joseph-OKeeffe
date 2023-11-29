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
	

	CheckPiece(GetCurrentPiece());
}

void Tile::Render(sf::RenderWindow& window)
{
	window.draw(tile);
	if (!isUsed)
	{
		window.draw(shape);
	}
}

void Tile::CheckPiece(Piece piece)
{
	SetShape(piece);
	SetColor(piece);
	tileName = tileShape + " " + tileColor;
	
}

void Tile::SetShape(Piece piece)
{
 	CheckCurrentPieceSize(piece);

	if (piece >= 0 && piece < 6)
	{
		shape.setRotation(45);
	}
	else
	{
		shape.setRotation(0);
	}

	if (piece >= 0 && piece < 6)
	{
		currentShape = SQUARE;
		tileShape = "Square";
		shapeSides = 4;  // Square
	}
	else if (piece >= 6 && piece < 12)
	{
		currentShape = CIRCLE;
		tileShape = "Circle";
		shapeSides = 100;  // Circle
	}
	else if (piece >= 12 && piece < 18)
	{
		currentShape = TRIANGLE;
		tileShape = "Triangle";
		shapeSides = 3;  // Triangle
	}
	else if (piece >= 18 && piece < 24)
	{
		currentShape = DIAMOND;
		tileShape = "Diamond";
		shapeSides = 4;  // Diamond
	}
	else if (piece >= 24 && piece < 30)
	{
		currentShape = HEXAGON;
		tileShape = "Hexagon";
		shapeSides = 6;  // Hexagon
	}
	else if (piece >= 30 && piece < 36)
	{
		currentShape = OCTAGON;
		tileShape = "Octagon";
		shapeSides = 8;  // Octagon
	}
	else
	{
		currentShape = NO_SHAPE;
		tileShape = "Used";
		shapeSides = 0;
	}

	shape.setPointCount(shapeSides);
}

void Tile::SetColor(Piece piece)
{
	switch (piece % 6)
	{
	case 0: shapeColor = sf::Color::Red; 
		currentColor = RED;
		tileColor = "Red";
		break;
	case 1: shapeColor = sf::Color::Green;
		currentColor = GREEN;
		tileColor = "Green";
		break;
	case 2: shapeColor = sf::Color::Blue;
		currentColor = BLUE;
		tileColor = "Blue";
		break;
	case 3: shapeColor = sf::Color::Yellow;
		currentColor = YELLOW;
		tileColor = "Yellow";
		break;
	case 4: shapeColor = sf::Color::Magenta; 
		currentColor = PINK;
		tileColor = "Pink";
		break;
	case 5: shapeColor = sf::Color::White; 
		currentColor = WHITE;
		tileColor = "White";
		break;
	default: shapeColor = sf::Color::Transparent; 
		currentColor = NO_COLOR;
		tileColor = "None";
		break;
	}

	shape.setFillColor(shapeColor);
}

void Tile::SetPiece(int piece)
{
	currentPiece = static_cast<Piece>(piece);
	CheckCurrentPieceSize(GetCurrentPiece());
	isPlaced = true;
	CheckPiece(GetCurrentPiece());
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

Color Tile::GetCurrentColor()
{
	return currentColor;
}

Shape Tile::GetCurrentShape()
{
	return currentShape;
}

void Tile::SetUsed()
{
	isUsed = true;
	CheckPiece(GetCurrentPiece());
}

bool Tile::GetUsed()
{
	return isUsed;
}

void Tile::ResetTile()
{
	isUsed = false;
}

void Tile::CheckCurrentPieceSize(int piece)
{
	if (piece >= 36)
	{
		int tooBig = 0;
		tooBig = piece;
		tooBig -= 36;
		currentPiece = static_cast<Piece>(tooBig);
		CheckCurrentPieceSize(GetCurrentPiece());
	}
}



