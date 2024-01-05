
#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Global.h"

#define _Output_To_Screen(x) std::cout << x << "\n";

enum GameState
{
	MENU,GAME
};

enum Difficulty
{
	EASY, MEDIUM, HARD
};



class Game
{
public:
	Game();
	~Game();

	void run();

private:

	bool isSinglePlayer = true;
	GameState state = MENU;
	Difficulty difficulty = EASY;
	int shuffleCounter = 0;

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

	void StartGame();
	int CheckWhoGoesFirst(Tile tile[6]);

	//void SelectPlayerTile();
	//void PlaceTileOnBoard();
	void SelectPlayerTile(Tile tiles[6]);
	void PlaceTileOnBoard(Tile tiles[6]);

	void NextTurn();
	void RefillPlayerAndAITiles();

	sf::Vector2i GetCellPosFromTile(Tile tile);
	std::vector<sf::Vector2i> GetCellPosFromTileList(std::vector<Tile> tiles);
	// Rules
	bool isValidPlacement = false;

	std::vector<Tile> GetValidNeighbours(int row, int col);
	//bool CheckValidTileColorOrShape(std::vector<Tile> neighbours);
	bool CheckValidTileColorOrShape(std::vector<Tile> neighbours, Tile tiles[6]);
	bool CheckFurtherInLine(Tile playerTile, Tile validTile);
	void SetTurnColourAndShape(Color color, Shape shape);
	//void ShufflePlayerTiles();
	void ShufflePlayerTiles(Tile tiles[6]);
	int CheckIfPlacingInSameLine(int row, int col);
	int AddScoresForTurn(std::vector<Tile> tilesToAddUp);

	sf::Vector2i sameLineVector;
	int currentLineNumber;
	bool isSameColumn = false;
	bool isSameRow = false;

	// AI
	bool aiCanPlaceTile = false;

	void SortAiTurn();
	sf::Vector2i MakeAiMove();
	sf::Vector2i GetAiMove();
	std::vector<Tile> FindPiecesOnBoard();
	void AiPlaceTileOnBoard(sf::Vector2i boardPos);
	std::vector<Tile> Game::GetAiNeighbours(int row, int col);
	bool CheckValidTileOrShapeAi(Tile tile);
	std::vector<Tile> GetValidNieghboursAi(std::vector<Tile> neighbours);

	std::vector<Tile> allPiecesOnBoard;
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
	Tile tilesInCurrentLineForScore[6];
	int selectedTile = 0;
	sf::Vector2i previousPlacedTile;
	int movesInTurnCount = 0;
	Tile firstTilePlacedInTurn;
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
	sf::Texture bagTexture;
	sf::Texture insideBagTexture;
	sf::Texture tableTexture;
	sf::Texture tileTexture;
	sf::Texture playerNameTexture;
	sf::Texture aiNameTexture;

	sf::Sprite playerShuffleButton;
	sf::Sprite endTurnButton;
	sf::Sprite bagButton;
	sf::Sprite tableSprite;
	sf::Text playerNameText;
	sf::Text aiNameText;
	sf::Text qwirkleText;
	std::string qwirkleString = "";
	void SetQwirkleText(std::string text);

	int playerScore = 0;
	int aiScore = 0;
	int scoreForCurrentTurn = 0;
	sf::Font font;
	sf::Text playerScoreText;
	sf::Text aiScoreText;
	std::vector<Tile> tilesPlacedInTurnForScore;

	std::vector<sf::Vector2i> validAiMoves;
	std::vector<Tile> validAiTiles;
	sf::Clock qwirkleClock;
	sf::Time qwirkleTimer;

	sf::Text qwirkleHeadingText;
	sf::RectangleShape easyButton;
	sf::Text easyText;
	sf::RectangleShape mediumButton;
	sf::Text mediumText;
	sf::RectangleShape hardButton;
	sf::Text hardText;


	sf::CircleShape pvpButton;
	sf::Text pvpText;
	sf::CircleShape pveButton;
	sf::Text pveText;
};

#endif

