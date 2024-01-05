#include "Tile.h"
#include <iostream>

Tile::Tile()
{
}

/// <summary>
/// Sets up all the data for the tile
/// </summary>
/// <param name="position">Takes a position of where the tile will be placed</param>
void Tile::Init(sf::Vector2f& position)
{

	tile.setFillColor(sf::Color::Transparent);
	tile.setSize(sf::Vector2f(Global::CELL_SIZE, Global::CELL_SIZE));
	tile.setOutlineThickness(1);
	tile.setOutlineColor(sf::Color(255,255,255, 100));
	tile.setPosition(position);

	shape.setRadius(tile.getSize().x / 2.5);
	shape.setOrigin(shape.getRadius(), shape.getRadius());
	shape.setPosition(position.x + Global::CELL_SIZE / 2, position.y + Global::CELL_SIZE / 2);
	shape.setOutlineThickness(1);
	shape.setOutlineColor(sf::Color::Black);

	tileShape = "None";
	tileColor = "None";
	

	CheckPiece(GetCurrentPiece());
}

/// <summary>
/// Draws everything on the screen
/// </summary>
/// <param name="window"></param>
void Tile::Render(sf::RenderWindow& window)
{
	window.draw(tile);
	if (!isUsed)
	{
		window.draw(shape);
	}
}

/// <summary>
/// Checks what the piece or shape/color of the tile is and sets its color and shape
/// </summary>
/// <param name="piece">Takes a piece</param>
void Tile::CheckPiece(Piece piece)
{
	SetShape(piece);
	SetColor(piece);
	tileName = tileColor+ " " + tileShape;
	
}

/// <summary>
/// Sets the shape of the tile using the piece
/// </summary>
/// <param name="piece">Piece is just an enum between 0 and 36</param>
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
		currentShape = EMPTY_S;
		tileShape = "Used";
		shapeSides = 0;
	}

	shape.setPointCount(shapeSides);
}

/// <summary>
/// Sets the color of the shape on the tile
/// </summary>
/// <param name="piece">Uses the piece to do this</param>
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
		currentColor = EMPTY_C;
		tileColor = "None";
		break;
	}

	shape.setFillColor(shapeColor);
}

/// <summary>
/// Sets the piece of the tile using an int
/// </summary>
/// <param name="piece">Int of the piece</param>
void Tile::SetPiece(int piece)
{
	currentPiece = static_cast<Piece>(piece);
	CheckCurrentPieceSize(GetCurrentPiece());
	isPlaced = true;
	CheckPiece(GetCurrentPiece());
}

/// <summary>
/// When the player chooses a tile this happens
/// </summary>
void Tile::SelectTile()
{
	tile.setFillColor(sf::Color(252, 3, 90, 230));
	shape.setOutlineThickness(1.5);
	isSelected = true;
}

/// <summary>
/// When the player places a tile or chooses another tile it deselects the last tile
/// </summary>
void Tile::DeselectTile()
{
	tile.setFillColor(sf::Color::White);
	shape.setOutlineThickness(1);
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

bool Tile::GetUsed()
{
	return isUsed;
}

/// <summary>
///  Sets the tile as used
/// </summary>
void Tile::SetUsed()
{
	isUsed = true;
	CheckPiece(GetCurrentPiece());
}

/// <summary>
/// Resets the tiles used back to false
/// </summary>
void Tile::ResetTile()
{
	isUsed = false;
}

/// <summary>
/// This is used for converting the bag of 108 tiles down to a value between 0 and 36
/// </summary>
/// <param name="piece">Takes the int that might be larger than 36</param>
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



