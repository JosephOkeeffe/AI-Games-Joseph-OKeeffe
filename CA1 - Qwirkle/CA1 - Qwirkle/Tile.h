#pragma once
#include <SFML/Graphics.hpp>
#include "Global.h"

enum Piece
{
	SR, //0 - SQUARE
	SG, //1
	SB, //2
	SY, //3
	SP, //4
	SW, //5

	CR,	//6 - CIRCLE
	CG,	//7
	CB,	//8
	CY,	//9
	CP,	//10
	CW,	//11
		
	TR,	//12 - TRIANGLE
	TG,	//13
	TB,	//14
	TY,	//15
	TP,	//16
	TW,	//17
		
	DR,	//18 - DIAMOND
	DG,	//19
	DB,	//20
	DY,	//21
	DP,	//22
	DW,	//23
		
	HR,	//24 - HEXAGON
	HG,	//25
	HB,	//26
	HY,	//27
	HP,	//28
	HW,	//29
		
	OR,	//30 - OCTAGON
	OG,	//31
	OB,	//32
	OY,	//33
	OP,	//34
	OW	//35
};

enum Color
{
	EMPTY_C,
	RED,
	GREEN,
	BLUE,
	YELLOW,
	PINK,
	WHITE
};
enum Shape
{
	EMPTY_S,
	SQUARE,
	CIRCLE,
	TRIANGLE,
	DIAMOND,
	HEXAGON,
	OCTAGON
};

class Tile
{
public:

	bool operator<(const Tile& other) const {
		// check by piece
		if (currentPiece != other.currentPiece) {
			return currentPiece < other.currentPiece;
		}
		// check by colour
		if (currentColor != other.currentColor) {
			return currentColor < other.currentColor;
		}
		// check by shape
		return currentShape < other.currentShape;
	}


	Tile();
	void Init(sf::Vector2f& position);
	void Render(sf::RenderWindow& window);

	void CheckPiece(Piece piece);


	void SetShape(Piece piece);
	void SetColor(Piece piece);

	void SetPiece(int piece);
	void SelectTile();
	void DeselectTile();
	Piece GetCurrentPiece();
	Color GetCurrentColor();
	Shape GetCurrentShape();
	void SetUsed();
	bool GetUsed();
	void ResetTile();

	void CheckCurrentPieceSize(int piece);



	std::string tileName;
	std::string tileColor;
	std::string tileShape;
	// Player / AI tiles
	bool isSelected = false;
	// Tile Pool
	bool isUsed = false;
	// Board
	bool isPlaced = false;

	bool isPlayer = false;
	bool isAI = false;

	sf::RectangleShape tile;

	int shapeSides = 0;
	sf::Color shapeColor;
	sf::CircleShape shape{25};

private:
	Piece currentPiece;
	Color currentColor;
	Shape currentShape;

};

