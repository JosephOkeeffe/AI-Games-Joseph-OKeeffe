
#include "Game.h"
#include <iostream>
#include <random>

Game::Game() :
	m_window{ sf::VideoMode{ Global::S_WIDTH, Global::S_HEIGHT + 100, 32U }, "Qwirkle" },
	m_exitGame{false}
{
	srand(time(nullptr));
	init();

}


Game::~Game()
{
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if ( sf::Event::Closed == newEvent.type)
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) 
		{
			processKeys(newEvent);
		}
		if (sf::Event::MouseButtonPressed == newEvent.type)
		{
			ProcessMouseDown(newEvent);
		}
	}
}

void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
	if (sf::Keyboard::Space == t_event.key.code)
	{
	
	}
	if (sf::Keyboard::Num1 == t_event.key.code)
	{
		NextTurn();
	}
	//if (sf::Keyboard::Num2 == t_event.key.code)
	//{
	//	OutputPlayerTile();
	//}
	//if (sf::Keyboard::Num3 == t_event.key.code)
	//{
	//	OutputAITiles();
	//}
	if (sf::Keyboard::B == t_event.key.code)
	{
		isBagOpen = !isBagOpen;
	}
}

void Game::ProcessMouseDown(sf::Event t_event)
{
	if (sf::Mouse::Left == t_event.key.code)
	{
		SelectPlayerTile();
		PlaceTileOnBoard();
		
	}
	
}

void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
}

void Game::render()
{
	m_window.clear(sf::Color(0, 0, 255, 100));

	for (int row = 0; row < Global::ROWS_COLUMNS; row++)
	{
		for (int col = 0; col < Global::ROWS_COLUMNS; col++)
		{
			board[row][col].Render(m_window);
		}
	}

	for (int i = 0; i < 6; i++)
	{
		playerTiles[i].Render(m_window);
	}

	for (int i = 0; i < 6; i++) 
	{
		aiTiles[i].Render(m_window);
	}

	if (isBagOpen)
	{
		m_window.draw(bagRect);
		for (int i = 0; i < 108; i++)
		{
			totalTilePool[i].Render(m_window);
		}
	}
	m_window.display();
}

void Game::init()
{
	srand(time(nullptr));
	InitBoard();
	SetupTilePool();
	SetupPlayerTiles();
	SetupAITiles();
	StartGame();
}

void Game::InitBoard()
{
	// BOARD
	board = new Tile * [Global::ROWS_COLUMNS];

	for (int i = 0; i < Global::ROWS_COLUMNS; i++)
	{
		board[i] = new Tile[Global::ROWS_COLUMNS];
	}

	for (int row = 0; row < Global::ROWS_COLUMNS; row++)
	{
		for (int col = 0; col < Global::ROWS_COLUMNS; col++)
		{
			board[row][col].Init(sf::Vector2f(row * Global::CELL_SIZE, col * Global::CELL_SIZE));
		}
	}
}

void Game::SetupTilePool()
{
	// Initialize tilePool2
	for (int i = 0; i < 36; i++)
	{
		float xPosition = (i % 6) * Global::CELL_SIZE + Global::S_HEIGHT * 0.35;
		float yPosition = Global::S_HEIGHT * 0.05 + (i / 6) * Global::CELL_SIZE;

		tilePool1[i].Init(sf::Vector2f(xPosition, yPosition));
		tilePool1[i].SetPiece(i);
	}

	// Initialize tilePool2
	for (int i = 0; i < 36; i++)
	{
		float xPosition = (i % 6) * Global::CELL_SIZE + Global::S_HEIGHT * 0.35;
		float yPosition = Global::S_HEIGHT * 0.05 + (i / 6) * Global::CELL_SIZE + Global::CELL_SIZE * 6;

		tilePool2[i].Init(sf::Vector2f(xPosition, yPosition));
		tilePool2[i].SetPiece(i);
	}

	// Initialize tilePool3
	for (int i = 0; i < 36; i++)
	{
		float xPosition = (i % 6) * Global::CELL_SIZE + Global::S_HEIGHT * 0.35;
		float yPosition = Global::S_HEIGHT * 0.05 + (i / 6) * Global::CELL_SIZE + Global::CELL_SIZE * 12;

		tilePool3[i].Init(sf::Vector2f(xPosition, yPosition));
		tilePool3[i].SetPiece(i);
	}

	std::copy(tilePool1, tilePool1 + 36, totalTilePool);
	std::copy(tilePool2, tilePool2 + 36, totalTilePool + 36);
	std::copy(tilePool3, tilePool3 + 36, totalTilePool + 72);

	bagRect.setPosition( Global::S_WIDTH * 0.3, 0);
	bagRect.setSize(sf::Vector2f(Global::CELL_SIZE * 8, Global::CELL_SIZE * 20));
	bagRect.setFillColor(sf::Color(139, 69, 19));
}

void Game::SetupPlayerTiles()
{
	int count = 0;
	while (count < 6)
	{
		playerTiles[count].Init(sf::Vector2f((count * Global::CELL_SIZE) + Global::S_HEIGHT * 0.1, Global::S_HEIGHT * 1.01));
		int randomTileFromTilePool = rand() % 108;
		if (!totalTilePool[randomTileFromTilePool].GetUsed())
		{
			playerTiles[count].SetPiece(totalTilePool[randomTileFromTilePool].GetCurrentPiece());
			totalTilePool[randomTileFromTilePool].SetUsed();
			count++;
		}
	}
}

void Game::SetupAITiles()
{
	int count = 0;
	while (count < 6)
	{
		aiTiles[count].Init(sf::Vector2f((count * Global::CELL_SIZE) + Global::S_HEIGHT * 0.6, Global::S_HEIGHT * 1.01));
		int randomTileFromTilePool = rand() % 108;
		if (!totalTilePool[randomTileFromTilePool].GetUsed())
		{
			aiTiles[count].SetPiece(totalTilePool[randomTileFromTilePool].GetCurrentPiece());
			totalTilePool[randomTileFromTilePool].SetUsed();
			count++;
		}
	}
	
}

int Game::GetActiveTilePool()
{
	int random = rand() % 108;

	if (!totalTilePool[random].isUsed)
	{
		std::cout << totalTilePool[random].tileName << " has been added to player \n";
		
		
		int count = 0;
		for each (Tile tile in totalTilePool)
		{
			if (tile.GetCurrentPiece() == totalTilePool[random].GetCurrentPiece() && !tile.isUsed)
			{
				count++;
			}
		}
		std::cout << "There are " << count - 1 << " " << totalTilePool[random].tileName << "'s left in the bag \n";
		return random;
	}
	else
	{
		GetActiveTilePool();
	}

}

int Game::TileCount()
{
	int count = 0;
	for each (Tile temp in totalTilePool)
	{
		if (!temp.GetUsed())
		{
			count++;
		}
	}

	std::cout << "Total Tile Count: " << count << "\n";
	return count;
}
void Game::OutputTilePool()
{
	std::cout << "-------------------------------------------" << "\n";
	std::cout << "Tile Pool" << "\n";
	std::cout << "-------------------------------------------" << "\n";

	for each (Tile tile in totalTilePool)
	{
		if (!tile.GetUsed())
		{
			std::cout << "Tile Type: " << tile.tileName << "\n";

		}
		else
		{
			std::cout << "Tile Type: " << tile.tileName << " -- USED" << "\n";
		}
		std::cout << "Tile Piece: " << tile.GetCurrentPiece() << "\n";
	}
}

void Game::OutputPlayerTile()
{
	std::cout << "-------------------------------------------" << "\n";
	std::cout << "Player Tiles" << "\n";
	std::cout << "-------------------------------------------" << "\n";

	for each (Tile tile in playerTiles)
	{
		std::cout << "Tile Type: " << tile.tileName << "\n";
		std::cout << "Tile Piece: " << tile.GetCurrentPiece() << "\n";

	}
}

void Game::OutputAITiles()
{
	std::cout << "-------------------------------------------" << "\n";
	std::cout << "AI Tiles" << "\n";
	std::cout << "-------------------------------------------" << "\n";

	for each (Tile tile in aiTiles)
	{
		std::cout << "Tile Type: " << tile.tileName << "\n";
		std::cout << "Tile Piece: " << tile.GetCurrentPiece() << "\n";

	}
}

void Game::StartGame()
{
	if (CheckWhoGoesFirst(playerTiles) > CheckWhoGoesFirst(aiTiles))
	{
		std::cout << "Player goes first \n";
	}
	else if (CheckWhoGoesFirst(playerTiles) < CheckWhoGoesFirst(aiTiles))
	{
		std::cout << "AI goes first \n";
	}
	else
	{
		int random = rand() % 2;

		std::cout << "Draw, random time.... \n";
		if (random == 0)
		{
			std::cout << "Player goes first \n";
		}
		else
		{
			std::cout << "AI goes first \n";
		}
	}
}

int Game::CheckWhoGoesFirst(Tile t_tiles[6])
{
	std::map<std::string, int> shapeCount;
	std::map<std::string, int> colorCount;

	int maxS = 0;
	for (int i = 0; i < 6; i++)
	{
		shapeCount[t_tiles[i].tileShape]++;
		colorCount[t_tiles[i].tileColor]++;
	}

	std::string maxShape;
	int maxShapeCount = 0;
	for (const auto& pair : shapeCount)
	{
		if (pair.second > maxShapeCount)
		{
			maxShape = pair.first;
			maxShapeCount = pair.second;
		}
	}

	std::string maxColor;
	int maxColorCount = 0;
	for (const auto& pair : colorCount)
	{
		if (pair.second > maxColorCount)
		{
			maxColor = pair.first;
			maxColorCount = pair.second;
		}
	}

	if (maxShapeCount > maxColorCount)
	{
		//std::cout << "The tile with the most common shape is: " << maxShape << " (" << maxShapeCount << " occurrences)\n";
		return maxShapeCount;
	}
	else if (maxColorCount > 0)
	{
		//std::cout << "The tile with the most common color is: " << maxColor << " (" << maxColorCount << " occurrences)\n";
		return maxColorCount;

	}
}

void Game::SelectPlayerTile()
{
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(Global::GetMousePos(m_window));

	for (int i = 0; i < 6; i++)
	{
		sf::FloatRect bounds = playerTiles[i].shape.getGlobalBounds();

		if (bounds.contains(mousePos))
		{

			if (!playerTiles[i].GetUsed())
			{
				// Delsecect old tile
				playerTiles[selectedTile].DeselectTile();
				// Select new tile
				playerTiles[i].SelectTile();
				// Get a new selected tile
				selectedTile = i;
				// Get the shape / piece from the tile you selected
				currentSelectedPiece = playerTiles[i].GetCurrentPiece();
				playerTiles[i].CheckPiece(currentSelectedPiece);
			}
		}
	}
}

void Game::PlaceTileOnBoard()
{
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(Global::GetMousePos(m_window));

	for (int row = 0; row < Global::ROWS_COLUMNS; row++)
	{
		for (int col = 0; col < Global::ROWS_COLUMNS; col++)
		{
			sf::FloatRect bounds = board[row][col].shape.getGlobalBounds();

			if (bounds.contains(mousePos))
			{
				isValidPlacement = CheckValidTileColorOrShape(CheckValidNeighbours(row, col));
				if (isValidPlacement || isFirstTurn)
				{
					isFirstTurn = false;
					board[row][col].SetPiece(currentSelectedPiece);
					SetTurnColourAndShape(playerTiles[selectedTile].GetCurrentColor(), playerTiles[selectedTile].GetCurrentShape());
					SameLineVector.push_back({ row,col });
					turnTiles.push_back(playerTiles[selectedTile]);
					// Deselecting the tile
					playerTiles[selectedTile].DeselectTile();
					playerTiles[selectedTile].SetUsed();
					std::cout << "You have placed : " << board[row][col].tileName << "\n";
				}
			}
		}

	}
}

	//sf::Vector2f mousePos = static_cast<sf::Vector2f>(Global::GetMousePos(m_window));

	//for (int row = 0; row < Global::ROWS_COLUMNS; row++)
	//{
	//	for (int col = 0; col < Global::ROWS_COLUMNS; col++)
	//	{
	//		sf::FloatRect bounds = board[row][col].shape.getGlobalBounds();

	//		if (bounds.contains(mousePos))
	//		{
	//			if (playerTiles[selectedTile].isSelected && !board[row][col].isPlaced)
	//			{
	//				if (CheckIfTileIsTouchingTileOfSameColourOrShape(row, col))
	//				{
	//					CheckTurnShapeAndColor();
	//					if (CheckTilesAreBeingPlacedInSameLine() == row || CheckTilesAreBeingPlacedInSameLine() == col || SameLineVector.size() < 2)
	//					{
	//						if (playerTiles[selectedTile].GetCurrentColor() == turnColor && isColorTurn || playerTiles[selectedTile].GetCurrentShape() == turnShape && isShapeTurn || turnTiles.size() < 2)
	//						{
	//							isFirstTurn = false;

	//							board[row][col].SetPiece(selectedPiece);

	//							SetTurnColourAndShape(playerTiles[selectedTile].GetCurrentColor(), playerTiles[selectedTile].GetCurrentShape());
	//							SameLineVector.push_back({ row,col });
	//							turnTiles.push_back(playerTiles[selectedTile]);
	//							// Deselecting the tile
	//							playerTiles[selectedTile].DeselectTile();
	//							playerTiles[selectedTile].SetUsed();
	//							std::cout << "You have placed : " << board[row][col].tileName << "\n";
	//						}
	//					}
	//				}

	//			}
	//		}

	//	}
	//}




void Game::PlacingRules()
{
	
}

// 9 neigbours, vector of them

//bool Game::CheckIfTileIsTouchingTileOfSameColourOrShape(int row, int col)
//{
//	bool canPlaceTile = false;
//
//	if (!isFirstTurn)
//	{
//		for (int i = row - 1; i <= row + 1; ++i)
//		{
//			for (int j = col - 1; j <= col + 1; ++j)
//			{
//				if (i == row && j == col) { continue; }
//
//				if (board[i][j].GetCurrentColor() == playerTiles[selectedTile].GetCurrentColor() && board[i][j].GetCurrentShape() == playerTiles[selectedTile].GetCurrentShape())
//				{
//					SameLineVector.push_back({ i,j });
//					canPlaceTile = false;
//					std::cout << "Cannot place exact same tile type beside each other \n";
//					return canPlaceTile;
//				}
//
//				if (i >= 0 && i < Global::ROWS_COLUMNS && j >= 0 && j < Global::ROWS_COLUMNS && (i == row || j == col))
//				{
//					if (board[i][j].GetCurrentColor() == playerTiles[selectedTile].GetCurrentColor() ||
//						board[i][j].GetCurrentShape() == playerTiles[selectedTile].GetCurrentShape())
//					{
//						canPlaceTile = true;
//						return canPlaceTile;
//					}
//				}
//				else
//				{
//					std::cout << "These tiles arent matching by Color or Shape\n";
//				}
//				
//				
//				
//			}
//		}
//	}
//	else
//	{
//		canPlaceTile = true;
//	}
//
//	return canPlaceTile;
//}

//int Game::CheckTilesAreBeingPlacedInSameLine()
//{
//	if (SameLineVector.size() < 2) return -1;
//	
//	sf::Vector2i firstPos = SameLineVector[0];
//	sf::Vector2i secondPos = SameLineVector[1];
//
//	int currentLine = 0;
//	if (firstPos.x == secondPos.x)
//	{
//		std::cout << "You have started placing tiles across so you cant place them down\n";
//		currentLine = firstPos.x;
//	}
//	else if (firstPos.y == secondPos.y)
//	{
//		std::cout << "You have started placing tiles down so you cant place them across\n";
//		currentLine = firstPos.y;
//	}
//	return currentLine;
//}

//void Game::CheckTurnShapeAndColor()
//{
//	if (turnTiles.size() < 2) return;
//
//	int colorCounter = 0;
//	int shapeCounter = 0;
//	for each (auto tile in turnTiles)
//	{
//		if (tile.GetCurrentColor() == turnColor) 
//		{
//			colorCounter++;
//		}
//		if (tile.GetCurrentShape() == turnShape)
//		{
//			shapeCounter++;
//		}
//	}
//
//	if (colorCounter > shapeCounter)
//	{
//		std::cout << "Can't place matching Shapes as you have matched Colors this turn \n";
//		isColorTurn = true;
//		isShapeTurn = false;
//	}
//	else
//	{
//		std::cout << "Can't place matching Colors as you have matched Shapes this turn \n";
//		isColorTurn = false;
//		isShapeTurn = true;
//	}
//}

void Game::SetTurnColourAndShape(Color color, Shape shape)
{
	if (turnColor == NO_COLOR)
	{
		turnColor = color;
	}
	if (turnShape == NO_SHAPE)
	{
		turnShape = shape;
	}
}

void Game::NextTurn()
{
	RefillPlayerAndAITiles();
	SameLineVector.clear();
	turnTiles.clear();
	isColorTurn = false;
	isShapeTurn = false;
	turnColor = NO_COLOR;
	turnShape = NO_SHAPE;
	currentTurn++;
}

void Game::RefillPlayerAndAITiles()
{
	for (int i = 0; i < 6; i++)
	{
		if (playerTiles[i].GetUsed())
		{
			playerTiles[i].ResetTile();
			int randomTileFromTilePool = GetActiveTilePool();
			// Refilling the players tile with the random piece
			playerTiles[i].SetPiece(randomTileFromTilePool);
			// Removing the tile from the tile pool
			totalTilePool[randomTileFromTilePool].SetUsed();
		}
	}
}

std::vector<Tile> Game::CheckValidNeighbours(int row, int col)
{
	std::vector<Tile> neighbourTiles;
	std::vector<Tile> validNeighbourTiles;

	
	for (int i = row - 1; i <= row + 1; ++i)
	{
		for (int j = col - 1; j <= col + 1; ++j)
		{
			if ((i == row && j == col) || (i != row && j != col)) { continue; }

			neighbourTiles.push_back(board[i][j]);
		}
	}


	for each (auto tile in neighbourTiles)
	{
		if (tile.GetCurrentColor() != NO_COLOR || tile.GetCurrentShape() != NO_SHAPE)
		{
			validNeighbourTiles.push_back(tile);
		}
	}


	return validNeighbourTiles;
}

bool Game::CheckValidTileColorOrShape(std::vector<Tile> validNeighbours)
{
	bool isValid = false;
	Tile tempPlayerTile = playerTiles[selectedTile];

	for each (auto tile in validNeighbours)
	{
		if (playerTiles[selectedTile].GetCurrentColor() == tile.GetCurrentColor() || playerTiles[selectedTile].GetCurrentShape() == tile.GetCurrentShape())
		{
			isValid = true;
			CheckFurtherInLine(playerTiles[selectedTile], tile);
		}
		else
		{
			isValid = false;
		}
	}
	return isValid;
}

bool Game::CheckFurtherInLine(Tile playerTile, Tile validTile)
{
	sf::Vector2i playerCell;
	playerCell.x = 	playerTile.tile.getPosition().x / Global::CELL_SIZE;
	playerCell.y = playerTile.tile.getPosition().y / Global::CELL_SIZE;

	sf::Vector2i validTileCell;
	validTileCell.x = validTile.tile.getPosition().x / Global::CELL_SIZE;
	validTileCell.y = validTile.tile.getPosition().y / Global::CELL_SIZE;

	if (playerCell.x == validTileCell.x)
	{
		std::cout << "Same DOWN\n";
	}

	return false;
}

