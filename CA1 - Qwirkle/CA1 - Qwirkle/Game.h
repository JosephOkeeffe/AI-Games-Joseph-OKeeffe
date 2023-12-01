
#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Global.h"

class Game
{
public:
	Game();
	~Game();

	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void ProcessMouseDown(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();
	void init();
	void InitBoard();
	void SetupTilePool();
	void SetupPlayerTiles();
	void SetupAITiles();
	int GetActiveTilePool();
	int TileCount();
	void OutputTilePool();
	void OutputPlayerTile();
	void OutputAITiles();

	void StartGame();
	int CheckWhoGoesFirst(Tile tile[6]);

	void SelectPlayerTile();
	void PlaceTileOnBoard();
	void PlacingRules();

	void NextTurn();
	void RefillPlayerAndAITiles();
	// Rules
	bool isValidPlacement = false;

	std::vector<Tile> CheckValidNeighbours(int row, int col);
	bool CheckValidTileColorOrShape(std::vector<Tile> neighbours);
	bool CheckFurtherInLine(Tile playerTile, Tile validTile);
	//bool CheckColourAndShape();

	//bool CheckIfTileIsTouchingTileOfSameColourOrShape(int row, int col);
	//int CheckTilesAreBeingPlacedInSameLine();
	//void CheckTurnShapeAndColor();
	void SetTurnColourAndShape(Color color, Shape shape);

	int currentTurn = 1;


	sf::RenderWindow m_window; 
	sf::Font m_ArialBlackfont;

	Tile** board;
	sf::RectangleShape bagRect;
	bool isBagOpen = false;

	Tile tilePool1[36]; // 108 (36 x 3)
	Tile tilePool2[36];
	Tile tilePool3[36];
	

	Tile totalTilePool[108];

	Tile playerTiles[6]; // 6
	Tile aiTiles[6]; // 6


	//std::vector<Tile> tilePool2;
	//std::vector<Tile> tilePool3;
	//std::vector<Tile> totalTilePool;

	//std::vector<Tile> playerTiles;
	//std::vector<Tile> aiTiles;

	Piece currentSelectedPiece;

	// Turns
	std::vector<sf::Vector2i> SameLineVector;
	std::vector<Tile> turnTiles;
	Color turnColor;
	Shape turnShape;
	bool isColorTurn = false;
	bool isShapeTurn = false;

	bool isFirstTurn = true;
	int selectedTile = 0;
	bool m_exitGame; 



};

#endif

