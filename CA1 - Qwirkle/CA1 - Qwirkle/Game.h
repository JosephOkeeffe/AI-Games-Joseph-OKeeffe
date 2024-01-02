
#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Global.h"

#define _Output_To_Screen(x) std::cout << x << "\n";

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
	void SetupSprites();
	void SetupAITiles();
	int GetActiveTilePool();
	int TileCount();

	void StartGame();
	int CheckWhoGoesFirst(Tile tile[6]);

	void SelectPlayerTile();
	void PlaceTileOnBoard();

	void NextTurn();
	void RefillPlayerAndAITiles();

	sf::Vector2i GetTileCell(Tile tile);
	// Rules
	bool isValidPlacement = false;

	std::vector<Tile> CheckValidNeighbours(int row, int col);
	bool CheckValidTileColorOrShape(std::vector<Tile> neighbours);
	bool CheckFurtherInLine(Tile playerTile, Tile validTile);
	bool CheckNeighbourIsInLine(int row, int col);
	void SetTurnColourAndShape(Color color, Shape shape);
	void ShufflePlayerTiles();
	int CheckIfPlacingInSameLine(int row, int col);
	sf::Vector2i sameLineVector;
	int currentLineNumber;
	bool isSameColumn = false;
	bool isSameRow = false;

	// AI
	bool aiCanPlaceTile = false;

	void SortAiTurn();
	sf::Vector2i MakeAiMove();
	sf::Vector2i GetAiMove();
	void AiPlaceTileOnBoard(sf::Vector2i boardPos);
	std::vector<Tile> Game::GetAiNeighbours(int row, int col);
	bool CheckValidTileOrShapeAi(std::vector<Tile> neighbours);

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
	Tile playerTiles[6];
	Tile aiTiles[6]; 



	// Turns
	Color turnColor;
	Shape turnShape;
	bool isColorTurn = false;
	bool isShapeTurn = false;
	bool isFirstMoveOfGame = true;


	// Player
	Piece currentSelectedPiece;
	sf::Vector2i currentCellPos;
	//std::vector<Tile> tilesInCurrentLine;
	Tile tilesInCurrentLineForScore[6];
	int selectedTile = 0;
	sf::Vector2i previousPlacedTile;
	int movesInTurnCount = 0;
	Tile firstTilePlacedInTurn;
	//std::vector<int> playerTileIdsForBag;
	int playerTileIdsForBag[6];


	bool m_exitGame;  
	// AI
	bool isPlayerTurn = true;
	Piece aiCurrentPiece;
	int aiSelectedTile = 0;
	int aiTileIdsForBag[6];
	

	sf::Clock aiClock;
	sf::Time aiTimer;

	sf::Texture shuffleTexture;
	sf::Texture endTurnTexture;
	sf::Sprite playerShuffleButton;
	sf::Sprite endTurnButton;


	int AddScoresForTurn();
	int playerScore = 0;
	int scoreForCurrentTurn = 0;
	sf::Font font;
	sf::Text playerScoreText;

};

#endif

