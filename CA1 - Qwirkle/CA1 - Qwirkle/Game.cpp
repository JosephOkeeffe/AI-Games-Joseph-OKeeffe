
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
		//processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
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
		if (sf::Event::MouseButtonPressed == newEvent.type && isPlayerTurn)
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
	if (m_exitGame)	{m_window.close();}

	processEvents();

	

	if (!isPlayerTurn) // AI  Turn
	{
		if (aiTimer.asSeconds() == 0) { aiClock.restart(); }
		aiTimer = aiClock.getElapsedTime();
		sf::Vector2i move = MakeAiMove();
		AiPlaceTileOnBoard(move);
		// 
		if (aiTimer.asSeconds() >= 2)
		{
			std::cout << "Players Turn!\n";
			NextTurn();
		}
		
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
	for (int i = 0; i < 36; i++)
	{
		float xPosition = (i % 6) * Global::CELL_SIZE + Global::S_HEIGHT * 0.35;
		float yPosition = Global::S_HEIGHT * 0.05 + (i / 6) * Global::CELL_SIZE;

		tilePool1[i].Init(sf::Vector2f(xPosition, yPosition));
		tilePool1[i].SetPiece(i);
	}

	for (int i = 0; i < 36; i++)
	{
		float xPosition = (i % 6) * Global::CELL_SIZE + Global::S_HEIGHT * 0.35;
		float yPosition = Global::S_HEIGHT * 0.05 + (i / 6) * Global::CELL_SIZE + Global::CELL_SIZE * 6;

		tilePool2[i].Init(sf::Vector2f(xPosition, yPosition));
		tilePool2[i].SetPiece(i);
	}

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
		isPlayerTurn = true;
		std::cout << "Player goes first \n";
	}
	else if (CheckWhoGoesFirst(playerTiles) < CheckWhoGoesFirst(aiTiles))
	{
		isPlayerTurn = false;
		std::cout << "AI goes first \n";
	}
	else
	{
		int random = rand() % 2;

		std::cout << "Draw, random time.... \n";
		if (random == 0)
		{
			isPlayerTurn = true;
			std::cout << "Player goes first \n";
		}
		else
		{
			isPlayerTurn = false;
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
				currentCellPos = { row, col };
				std::cout << "Row: " << row << ", " << col << "\n";

				isValidPlacement = CheckValidTileColorOrShape(CheckValidNeighbours(row, col));

				if (isValidPlacement || isFirstMoveOfGame)
				{
					isFirstMoveOfGame = false;
					previousPlacedTile = { row, col };
					lineTiles.push_back(playerTiles[selectedTile]);
					board[row][col].SetPiece(currentSelectedPiece);
					SetTurnColourAndShape(playerTiles[selectedTile].GetCurrentColor(), playerTiles[selectedTile].GetCurrentShape());
					playerTiles[selectedTile].DeselectTile();
					playerTiles[selectedTile].SetUsed();
					std::cout << "You have placed : " << board[row][col].tileName << "\n";

				}
			}
		}

	}
}

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
	//SameLineVector.clear();
	//firstMoveInTurn = true;
	lineTiles.clear();
	isColorTurn = false;
	isShapeTurn = false;
	turnColor = NO_COLOR;
	turnShape = NO_SHAPE;
	currentTurn++;
	isPlayerTurn = !isPlayerTurn;
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

	for (int i = 0; i < 6; i++)
	{
		if (aiTiles[i].GetUsed())
		{
			aiTiles[i].ResetTile();
			int randomTileFromTilePool = GetActiveTilePool();
			// Refilling the players tile with the random piece
			aiTiles[i].SetPiece(randomTileFromTilePool);
			// Removing the tile from the tile pool
			totalTilePool[randomTileFromTilePool].SetUsed();
		}
	}
}

sf::Vector2i Game::GetTileCell(Tile tile)
{
	sf::Vector2i tilePos;

	tilePos.x = tile.tile.getPosition().x / Global::CELL_SIZE;
	tilePos.y = tile.tile.getPosition().y / Global::CELL_SIZE;
	return tilePos;
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

			if (i >= 0 && i < Global::ROWS_COLUMNS 
			 && j >= 0 && j < Global::ROWS_COLUMNS)
			{
				neighbourTiles.push_back(board[i][j]);
			}
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
			isValid = CheckFurtherInLine(playerTiles[selectedTile], tile);
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
	bool isValid = false;
	sf::Vector2i playerCell;
	playerCell.x = 	currentCellPos.x;
	playerCell.y = currentCellPos.y;

	sf::Vector2i validTileCell = GetTileCell(validTile);

	int leftOfValidTile = validTileCell.x - 1;
	int rightOfValidTile = validTileCell.x + 1;
	int aboveValidTile = validTileCell.y - 1;
	int belowValidTile = validTileCell.y + 1;

	if (currentTurn > 1)
	{
		lineTiles.push_back(validTile);
	}

	if (playerCell.x == validTileCell.x)
	{
		std::cout << "Checking in VERTICAL line\n";
		if (validTileCell.y < playerCell.y)
		{
			if (aboveValidTile >= 0)
			{
				if (board[validTileCell.x][aboveValidTile].GetCurrentColor() == playerTile.GetCurrentColor())
				{
					std::cout << "Color is the same one to the ABOVE \n";
					isValid = true;
				}
				else if (board[validTileCell.x][aboveValidTile].GetCurrentColor() == NO_COLOR)
				{
					isValid = true;
 					if (lineTiles.size() >= 2)
					{
						isValid = false;
						isValid = CheckNieghbourIsInLine(playerCell.x, playerCell.y);

					}
				}
				else if (board[validTileCell.x][aboveValidTile].GetCurrentShape() == playerTile.GetCurrentShape())
				{
					std::cout << "Shape is the same one as ABOVE \n";
					isValid = true;
				}
				else if (board[validTileCell.x][aboveValidTile].GetCurrentShape() == NO_SHAPE)
				{
					isValid = true;
					if (lineTiles.size() >= 2)
					{
						isValid = false;
						isValid = CheckNieghbourIsInLine(playerCell.x, playerCell.y);
					}
				}
			}
		}
		else if (validTileCell.y > playerCell.y)
		{
			if (belowValidTile <= Global::ROWS_COLUMNS)
			{
				if (board[validTileCell.x][belowValidTile].GetCurrentColor() == playerTile.GetCurrentColor())
				{
					std::cout << "Color is the same one to the BELOW \n";
					isValid = true;
				}
				else if (board[validTileCell.x][belowValidTile].GetCurrentColor() == NO_COLOR)
				{
					isValid = true;
					if (lineTiles.size() >= 2)
					{
						isValid = false;
						isValid = CheckNieghbourIsInLine(playerCell.x, playerCell.y);

					}
				}
				else if (board[validTileCell.x][belowValidTile].GetCurrentShape() == playerTile.GetCurrentShape())
				{
					std::cout << "Shape is the same one BELOW \n";
					isValid = true;
				}
				else if (board[validTileCell.x][belowValidTile].GetCurrentShape() == NO_SHAPE)
				{
					isValid = true;
					if (lineTiles.size() >= 2)
					{
						isValid = false;
						isValid = CheckNieghbourIsInLine(playerCell.x, playerCell.y);

					}
				}
			}

			
		}
		
	}
	if (playerCell.y == validTileCell.y)
	{
		std::cout << "Checking in HORIZONTAL line\n";
		if (validTileCell.x < playerCell.x)
		{
			if (leftOfValidTile >= 0)
			{
				if (board[leftOfValidTile][validTileCell.y].GetCurrentColor() == playerTile.GetCurrentColor())
				{
					std::cout << "Color is the same one to the Left \n";
					isValid = true;
				}
				else if (board[leftOfValidTile][validTileCell.y].GetCurrentColor() == NO_COLOR)
				{
					isValid = true;
					if (lineTiles.size() >= 2)
					{
						isValid = false;
						isValid = CheckNieghbourIsInLine(playerCell.x, playerCell.y);

					}
				}
				else if (board[leftOfValidTile][validTileCell.y].GetCurrentShape() == playerTile.GetCurrentShape())
				{
					std::cout << "Shape is the same one to the Left \n";
					isValid = true;
				}
				else if (board[leftOfValidTile][validTileCell.y].GetCurrentShape() == NO_SHAPE)
				{
					isValid = true;
					if (lineTiles.size() >= 2)
					{
						isValid = false;
						isValid = CheckNieghbourIsInLine(playerCell.x, playerCell.y);

					}
				}
			}
		}
		else if (validTileCell.x > playerCell.x)
		{
			if (rightOfValidTile <= Global::ROWS_COLUMNS)
			{
				if (board[rightOfValidTile][validTileCell.y].GetCurrentColor() == playerTile.GetCurrentColor())
				{
					std::cout << "Color is the same one to the right \n";
					isValid = true;
				}
				else if (board[rightOfValidTile][validTileCell.y].GetCurrentColor() == NO_COLOR)
				{
					isValid = true;
					if (lineTiles.size() >= 2)
					{
						isValid = false;
						isValid = CheckNieghbourIsInLine(playerCell.x, playerCell.y);
					}
				}
				else if (board[rightOfValidTile][validTileCell.y].GetCurrentShape() == playerTile.GetCurrentShape())
				{
					std::cout << "Shape is the same one to the right \n";
					isValid = true;
				}
				else if (board[rightOfValidTile][validTileCell.y].GetCurrentShape() == NO_SHAPE)
				{
					isValid = true;
					if (lineTiles.size() >= 2)
					{
						isValid = false;
						isValid = CheckNieghbourIsInLine(playerCell.x, playerCell.y);

					}
				}
			}
		}

	}
	return isValid;
}

bool Game::CheckNieghbourIsInLine(int row, int col)
{
	bool isValid = true;

	sf::Vector2i validTileCell;

	for each (auto tile in CheckValidNeighbours(row, col))
	{

		validTileCell = GetTileCell(tile);
		if (validTileCell.x + 1 == currentCellPos.x)
		{
			isValid = false;
			return isValid;
		}
		else if (validTileCell.x - 1 == currentCellPos.x)
		{
			isValid = false;
			return isValid;
		}
		if (validTileCell.y + 1 == currentCellPos.y)
		{
			isValid = false;
			return isValid;
		}
		else if (validTileCell.y - 1 == currentCellPos.y)
		{
			isValid = false;
			return isValid;
		}
	}
	return isValid;
}

///////////////
///////////////
///////////////

sf::Vector2i Game::MakeAiMove()
{
	sf::Vector2i movePos;

	movePos = GetAiMove();

	if (movePos.x == -1)
	{
		std::cout << "No valid move, skipping turn \n";
	}

	return movePos;
}

sf::Vector2i Game::GetAiMove()
{
	sf::Vector2i movePos;
	sf::Vector2i foundPiece;
	std::vector<Tile> neighbourTiles;

	for (int row = 0; row < Global::ROWS_COLUMNS; row++)
	{
		for (int col = 0; col < Global::ROWS_COLUMNS; col++)
		{
			if (board[row][col].GetCurrentColor() != NO_COLOR)
			{
				foundPiece = { row, col };
				neighbourTiles = GetAiNeighbours(row, col);
				for each (Tile neighbour in neighbourTiles)
				{
					if (neighbour.GetCurrentColor() == NO_COLOR) 
					{
						
						movePos = GetTileCell(neighbour);
					}
				}
				//CheckValuesOfNeighbours(neighbourTiles);
			}
		}

	}

	//movePos = { 5, 5 };
	return movePos;
}

void Game::AiPlaceTileOnBoard(sf::Vector2i boardPos)
{
	currentCellPos = boardPos;
	int row = boardPos.x;
	int col = boardPos.y;

	aiCurrentPiece = aiTiles[0].GetCurrentPiece();
	// Do more stuff,
	
	if (!aiTiles[0].GetUsed())
	{
		board[row][col].SetPiece(aiCurrentPiece);

	}
	aiTiles[0].SetUsed();

}

std::vector<Tile> Game::GetAiNeighbours(int row, int col)
{
	std::vector<Tile> neighbourTiles;

	for (int i = row - 1; i <= row + 1; ++i)
	{
		for (int j = col - 1; j <= col + 1; ++j)
		{
			if ((i == row && j == col) || (i != row && j != col)) { continue; }

			if (i >= 0 && i < Global::ROWS_COLUMNS
				&& j >= 0 && j < Global::ROWS_COLUMNS)
			{
				neighbourTiles.push_back(board[i][j]);
			}
		}
	}
	return neighbourTiles;
}

void Game::CheckValuesOfNeighbours(std::vector<Tile> neighbours)
{
	for each (Tile neighbour in neighbours)
	{
		if (neighbour.GetCurrentColor() == NO_COLOR) 
		{
			aiCanPlaceTile = true;
		}
	}
}

