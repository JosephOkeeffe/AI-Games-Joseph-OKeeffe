#pragma once
#include <SFML/Graphics.hpp>
#include "Global.h"

enum Piece
{
	SR,
	SG,
	SB,
	SY,
	SP,
	SW,

	CR,
	CG,
	CB,
	CY,
	CP,
	CW,

	TR,
	TG,
	TB,
	TY,
	TP,
	TW,

	DR,
	DG,
	DB,
	DY,
	DP,
	DW,

	HR,
	HG,
	HB,
	HY,
	HP,
	HW,

	OR,
	OG,
	OB,
	OY,
	OP,
	OW,

	NONE
};

class Tile
{
public:

	Tile();
	void Init(sf::Vector2f& position);
	void Render(sf::RenderWindow& window);


	void CheckPiece();


	void SetShape();
	void SetColor();

	bool isPlayer = false;
	bool isAI = false;
	void SetRandomTile();
	

	Piece currentPiece;

	sf::RectangleShape tile;

	int shapeSides = 0;
	sf::Color shapeColor;
	sf::CircleShape shape{25};

};

