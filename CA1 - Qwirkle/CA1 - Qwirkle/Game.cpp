
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
		
		if (isPlayerTurn)
		{
			sf::Vector2f mousePos = static_cast<sf::Vector2f>(Global::GetMousePos(m_window));
			if (playerShuffleButton.getGlobalBounds().contains(mousePos)){ ShufflePlayerTiles(); }
			if (endTurnButton.getGlobalBounds().contains(mousePos)){ NextTurn(); }
		}
	}
	
}

void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)	{m_window.close();}

	aiTimer = aiClock.getElapsedTime();

	if (isPlayerTurn)
	{
		playerScoreText.setString(std::to_string(playerScore));
		processEvents();
	}
	else
	{
		if (aiTimer.asSeconds() >= 2)
		{
			SortAiTurn();
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

	m_window.draw(playerShuffleButton);
	m_window.draw(endTurnButton);
	m_window.draw(playerScoreText);
	m_window.display();
}

void Game::init()
{
	srand(time(nullptr));
	
	InitBoard();
	SetupTilePool();
	SetupPlayerTiles();
	SetupAITiles();
	SetupSprites();
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
		float xPosition = (i % 6) * Global::CELL_SIZE + Global::S_WIDTH * 0.35;
		float yPosition = Global::S_HEIGHT * 0.05 + (i / 6) * Global::CELL_SIZE;

		tilePool1[i].Init(sf::Vector2f(xPosition, yPosition));
		tilePool1[i].SetPiece(i);
	}

	for (int i = 0; i < 36; i++)
	{
		float xPosition = (i % 6) * Global::CELL_SIZE + Global::S_WIDTH * 0.35;
		float yPosition = Global::S_HEIGHT * 0.05 + (i / 6) * Global::CELL_SIZE + Global::CELL_SIZE * 6;

		tilePool2[i].Init(sf::Vector2f(xPosition, yPosition));
		tilePool2[i].SetPiece(i);
	}

	for (int i = 0; i < 36; i++)
	{
		float xPosition = (i % 6) * Global::CELL_SIZE + Global::S_WIDTH * 0.35;
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


void Game::SetupSprites()
{
	shuffleTexture.loadFromFile("ASSETS\\IMAGES\\shuffle.png");
	endTurnTexture.loadFromFile("ASSETS\\IMAGES\\turn.png");
	font.loadFromFile("ASSETS\\FONTS\\ariblk.ttf");

	playerShuffleButton.setTexture(shuffleTexture);
	playerShuffleButton.setScale(0.2, 0.2);
	playerShuffleButton.setPosition(Global::S_WIDTH * 0.025, playerTiles[0].tile.getPosition().y);	
	
	endTurnButton.setTexture(endTurnTexture);
	endTurnButton.setScale(0.2, 0.2);
	endTurnButton.setPosition(Global::S_WIDTH * 0.025, playerTiles[0].tile.getPosition().y + 45);

	playerScoreText.setFont(font);
	playerScoreText.setString(std::to_string(playerScore));
	playerScoreText.setCharacterSize(26);
	playerScoreText.setPosition(Global::S_WIDTH * 0.41, playerTiles[0].tile.getPosition().y + 5);

}

int Game::GetActiveTilePool()
{
	int random = rand() % 108;

	if (!totalTilePool[random].isUsed)
	{
		int count = 0;
		for each (Tile tile in totalTilePool)
		{
			if (tile.GetCurrentPiece() == totalTilePool[random].GetCurrentPiece() && !tile.isUsed)
			{
				count++;
			}
		}
		return random;
	}
	else
	{
		GetActiveTilePool();
	}

}

void Game::StartGame()
{
	if (CheckWhoGoesFirst(playerTiles) > CheckWhoGoesFirst(aiTiles))
	{
		isPlayerTurn = true;
		_Output_To_Screen("Player goes first");
	}
	else if (CheckWhoGoesFirst(playerTiles) < CheckWhoGoesFirst(aiTiles))
	{
		isPlayerTurn = false;
		_Output_To_Screen("AI goes first");
	}
	else
	{
		int random = rand() % 2;

		_Output_To_Screen("Draw, random time....");
		if (random == 0)
		{
			isPlayerTurn = true;
			_Output_To_Screen("Player goes first");
		}
		else
		{
			isPlayerTurn = false;
			_Output_To_Screen("AI goes first");
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

				isValidPlacement = CheckValidTileColorOrShape(GetValidNeighbours(row, col));
				if (isValidPlacement && movesInTurnCount != 0)
				{
					if (row == CheckIfPlacingInSameLine(row, col) || col == CheckIfPlacingInSameLine(row, col))
					{
						isValidPlacement = true;
					}
					else
					{
						isValidPlacement = false;
					}
				}

				if (isValidPlacement || isFirstMoveOfGame)
				{

					previousPlacedTile = { row, col };
					//linesPlacedInTurn.push_back(playerTiles[selectedTile]);
					board[row][col].SetPiece(currentSelectedPiece);
					SetTurnColourAndShape(playerTiles[selectedTile].GetCurrentColor(), playerTiles[selectedTile].GetCurrentShape());
					playerTiles[selectedTile].DeselectTile();
					playerTiles[selectedTile].SetUsed();
					std::cout << "You have placed : " << board[row][col].tileName << "\n";
					board[row][col].tile.setFillColor(sf::Color::Red);
					if (movesInTurnCount == 0)
					{
						firstTilePlacedInTurn = board[row][col];
						sameLineVector = { row, col };
					}

					movesInTurnCount++;
					isFirstMoveOfGame = false;

				}
			}
		}

	}
}

int Game::CheckIfPlacingInSameLine(int row, int col)
{
	if (movesInTurnCount == 1)
	{
		if (sameLineVector.x == row)
		{
			currentLineNumber = row;
		}
		else if (sameLineVector.y == col)
		{
			currentLineNumber = col;
		}
		else
		{
			_Output_To_Screen("The tile you are trying to place is not part of the same line");
			return -1;
		}
	}

	if (row == currentLineNumber)
	{
		for (int i = 1; i < 6; i++)
		{
		
			int sameLineCountDown = sameLineVector.y + i;
			int sameLineCountUp = sameLineVector.y - i;

			if (sameLineCountDown <= Global::ROWS_COLUMNS && 
				sameLineVector.y < col)
			{
				if (board[row][sameLineCountDown].GetCurrentColor() == EMPTY_C && 
					sameLineCountDown < col)
				{
					_Output_To_Screen("The tile you are trying to place is not part of the same line");
					return -1;
				}
			}
			if (sameLineCountUp >= 0 &&
				sameLineVector.y > col)
			{
				if (board[row][sameLineCountUp].GetCurrentColor() == EMPTY_C && 
					sameLineCountUp > col)
				{
					_Output_To_Screen("The tile you are trying to place is not part of the same line");
					return -1;
				}
			}

		}
	}

	if (col == currentLineNumber)
	{
		for (int i = 1; i < 6; i++)
		{
			int sameLineCountRight = sameLineVector.x + i;
			int sameLineCountLeft = sameLineVector.x - i;

			if (sameLineCountRight <= Global::ROWS_COLUMNS && sameLineVector.x < row)
			{
				if (board[sameLineCountRight][col].GetCurrentColor() == EMPTY_C && sameLineCountRight < row)
				{
					_Output_To_Screen("The tile you are trying to place is not part of the same line");
					return -1;
				}
			}
			if (sameLineCountLeft >= 0 && sameLineVector.x > row)
			{
				if (board[sameLineCountLeft][col].GetCurrentColor() == EMPTY_C && sameLineCountLeft > row)
				{
					_Output_To_Screen("The tile you are trying to place is not part of the same line");
					return -1;
				}
			}

		}
	}


	return currentLineNumber;
}

void Game::ShufflePlayerTiles()
{
	playerScore /= 2;

	for (int i = 0; i < 6; i++)
	{
		
		totalTilePool[playerTileIdsForBag[i]].ResetTile();
		playerTiles[i].SetUsed();
		playerTiles[i].DeselectTile();
	}
	RefillPlayerAndAITiles();
}

void Game::SetTurnColourAndShape(Color color, Shape shape)
{
	if (turnColor == EMPTY_C)
	{
		turnColor = color;
	}
	if (turnShape == EMPTY_S)
	{
		turnShape = shape;
	}
}

void Game::NextTurn()
{
	RefillPlayerAndAITiles();
	if(isPlayerTurn){ playerScore += AddScoresForTurn(); }
	
	movesInTurnCount = 0;
	isColorTurn = false;
	isShapeTurn = false;
	turnColor = EMPTY_C;
	turnShape = EMPTY_S;
	currentTurn++;
	isPlayerTurn = !isPlayerTurn;
	aiClock.restart();
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
			playerTileIdsForBag[i] = randomTileFromTilePool;
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
			aiTileIdsForBag[i] = randomTileFromTilePool;
		}
	}
}

sf::Vector2i Game::GetCellPosFromTile(Tile tile)
{
	sf::Vector2i tilePos;

	tilePos.x = tile.tile.getPosition().x / Global::CELL_SIZE;
	tilePos.y = tile.tile.getPosition().y / Global::CELL_SIZE;
	return tilePos;
}

std::vector<sf::Vector2i> Game::GetCellPosFromTileList(std::vector<Tile> tiles)
{
	std::vector<sf::Vector2i> tilePosVector;
	sf::Vector2i tilePos;

	for each (Tile tile in tiles)
	{
		tilePos.x = tile.tile.getPosition().x / Global::CELL_SIZE;
		tilePos.y = tile.tile.getPosition().y / Global::CELL_SIZE;

		tilePosVector.push_back(tilePos);
	}


	return tilePosVector;
}

std::vector<Tile> Game::GetValidNeighbours(int row, int col)
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
		if (tile.GetCurrentColor() != EMPTY_C || tile.GetCurrentShape() != EMPTY_S)
		{
			validNeighbourTiles.push_back(tile);
		}
	}

	return validNeighbourTiles;
}

bool Game::CheckValidTileColorOrShape(std::vector<Tile> validNeighbours)
{
	bool isValid = false;
	Tile selectedPlayerTile = playerTiles[selectedTile];

	for each (auto tile in validNeighbours)
	{
		if (selectedPlayerTile.GetCurrentColor() == tile.GetCurrentColor() || selectedPlayerTile.GetCurrentShape() == tile.GetCurrentShape())
		{
			isValid = CheckFurtherInLine(selectedPlayerTile, tile);
		}
		else
		{
			isValid = false;
			return isValid;
		}
	}
	return isValid;
}

bool Game::CheckFurtherInLine(Tile playerTile, Tile validTile)
{
	bool isValid = false;
	sf::Vector2i playerCell = currentCellPos;
	sf::Vector2i validTileCell = GetCellPosFromTile(validTile);

	int leftOfValidTile = validTileCell.x - 1;
	int rightOfValidTile = validTileCell.x + 1;
	int aboveValidTile = validTileCell.y - 1;
	int belowValidTile = validTileCell.y + 1;

	leftOfValidTile = (leftOfValidTile < 0) ? 0 : leftOfValidTile;
	rightOfValidTile = (rightOfValidTile > Global::ROWS_COLUMNS) ? Global::ROWS_COLUMNS : rightOfValidTile;
	aboveValidTile = (aboveValidTile < 0) ? 0 : aboveValidTile;
	belowValidTile = (belowValidTile > Global::ROWS_COLUMNS) ? Global::ROWS_COLUMNS : belowValidTile;

	if (playerCell.x == validTileCell.x)
	{
		if (validTileCell.y < playerCell.y)
		{
			// CHECK IN A LINE GOING UP, IF THE COLORS MATCH. STOPS WHEN IT FINDS AN EMPTY TILE
			for (int i = 0; validTileCell.y - i >= 0 && board[validTileCell.x][validTileCell.y - i].GetCurrentColor() != EMPTY_C; i++)
			{
				//validTileCell.y = (validTileCell.y - i < 0) ? 0 : validTileCell.y;
				int tilesInLineCountX = validTileCell.x - i;
				int tilesInLineCountY = validTileCell.y - i;
				
				// CANT HAVE THE EXACT SAME TILE IN THE SAME LINE
				if (board[validTileCell.x][tilesInLineCountY].GetCurrentPiece() == playerTile.GetCurrentPiece() ||
					board[tilesInLineCountY][validTileCell.y].GetCurrentPiece() == playerTile.GetCurrentPiece())
				{
					_Output_To_Screen("Warning: Cant place two of the same tiles in the same line");

					isValid = false;
					return isValid;
				}
				else
				{
					if (board[validTileCell.x][tilesInLineCountY].GetCurrentColor() == playerTile.GetCurrentColor())
					{
						isValid = true;
					}
					else if (board[validTileCell.x][tilesInLineCountY].GetCurrentShape() == playerTile.GetCurrentShape())
					{
						isValid = true;
					}
					else
					{
						isValid = false;
						return isValid;
					}
				}
			}
			 if (board[validTileCell.x][aboveValidTile].GetCurrentShape() == EMPTY_S ||
				board[validTileCell.x][aboveValidTile].GetCurrentColor() == EMPTY_C)
			{
				isValid = true;
			}

		}
		else if (validTileCell.y > playerCell.y)
		{
			// CHECK IN A LINE GOING UP, IF THE COLORS MATCH. STOPS WHEN IT FINDS AN EMPTY TILE
			for (int i = 0; validTileCell.y + i < Global::ROWS_COLUMNS && board[validTileCell.x][validTileCell.y + i].GetCurrentColor() != EMPTY_C; i++)
			{
				// CANT HAVE THE EXACT SAME TILE IN THE SAME LINE
				if (board[validTileCell.x][validTileCell.y + i].GetCurrentPiece() == playerTile.GetCurrentPiece())
				{
					_Output_To_Screen("Warning: Cant place two of the same tiles in the same line");

					isValid = false;
					return isValid;
				}
				else
				{
					if (board[validTileCell.x][validTileCell.y + i].GetCurrentColor() == playerTile.GetCurrentColor())
					{
						isValid = true;
					}
					else if (board[validTileCell.x][validTileCell.y + i].GetCurrentShape() == playerTile.GetCurrentShape())
					{
						isValid = true;
					}
					else
					{
						isValid = false;
						return isValid;
					}

				}
			}
			if (board[validTileCell.x][belowValidTile].GetCurrentShape() == EMPTY_S ||
				board[validTileCell.x][belowValidTile].GetCurrentColor() == EMPTY_C)
			{
				isValid = true;
			}
		}
	}
	if (playerCell.y == validTileCell.y)
	{
		if (validTileCell.x < playerCell.x)
		{
			// CHECK IN A LINE GOING UP, IF THE COLORS MATCH. STOPS WHEN IT FINDS AN EMPTY TILE
			for (int i = 0; validTileCell.x - i >= 0 && board[validTileCell.x - i][validTileCell.y].GetCurrentColor() != EMPTY_C; i++)
			{
				// CANT HAVE THE EXACT SAME TILE IN THE SAME LINE
				if (board[validTileCell.x - i][validTileCell.y].GetCurrentPiece() == playerTile.GetCurrentPiece())
				{
					_Output_To_Screen("Warning: Cant place two of the same tiles in the same line");

					isValid = false;
					return isValid;
				}
				else
				{
					if (board[validTileCell.x - i][validTileCell.y].GetCurrentColor() == playerTile.GetCurrentColor())
					{
						isValid = true;
					}
					else if (board[validTileCell.x - i][validTileCell.y].GetCurrentShape() == playerTile.GetCurrentShape())
					{
						isValid = true;
					}
					else
					{
						isValid = false;
						return isValid;
					}
				}
			}
		}
		else if (validTileCell.x > playerCell.x)
		{
			for (int i = 0; validTileCell.x + i < Global::ROWS_COLUMNS && board[validTileCell.x + i][validTileCell.y].GetCurrentColor() != EMPTY_C; i++)
			{
				// CANT HAVE THE EXACT SAME TILE IN THE SAME LINE
				if (board[validTileCell.x + i][validTileCell.y].GetCurrentPiece() == playerTile.GetCurrentPiece())
				{
					_Output_To_Screen("Warning: Cant place two of the same tiles in the same line");

					isValid = false;
					return isValid;
				}
				else
				{
					if (board[validTileCell.x + i][validTileCell.y].GetCurrentColor() == playerTile.GetCurrentColor())
					{
						isValid = true;
					}
					else if (board[validTileCell.x + i][validTileCell.y].GetCurrentShape() == playerTile.GetCurrentShape())
					{
						isValid = true;
					}
					else
					{
						isValid = false;
						return isValid;
					}
				}
			}
		}
	}

	return isValid;
}

int Game::AddScoresForTurn()
{
	int counter = 1;

	sf::Vector2i startPos = GetCellPosFromTile(firstTilePlacedInTurn);
	for (int i = 1; startPos.x - i > 0 && board[startPos.x - i][startPos.y].GetCurrentColor() != EMPTY_C; i++)
	{
		counter++;
	}
	for (int i = 1; startPos.x + i < Global::ROWS_COLUMNS && board[startPos.x + i][startPos.y].GetCurrentColor() != EMPTY_C; i++)
	{
		counter++;
	}

	for (int i = 1; startPos.y - i > 0 && board[startPos.x][startPos.y - i].GetCurrentColor() != EMPTY_C; i++)
	{
		counter++;
	}
	for (int i = 1; startPos.y + i < Global::ROWS_COLUMNS && board[startPos.x][startPos.y + i].GetCurrentColor() != EMPTY_C; i++)
	{
		counter++;
	}

	return counter;
}

///////////////
///////////////
///////////////

void Game::SortAiTurn()
{
	sf::Vector2i move;

	if (isFirstMoveOfGame)
	{
		move = { rand() % Global::ROWS_COLUMNS,rand() % Global::ROWS_COLUMNS };
		sameLineVector = move;
		AiPlaceTileOnBoard(move);
		isFirstMoveOfGame = false;
		
	}
	else
	{
		move = MakeAiMove();

		if (move.x == -1)
		{
			_Output_To_Screen("AI has no valid move: skipping turn");
			NextTurn();
			return;
		}
		else
		{
			AiPlaceTileOnBoard(move);
		}
	}
	movesInTurnCount++;
	aiClock.restart();

}

sf::Vector2i Game::GetAiMove()
{
	sf::Vector2i movePos = { -1, -1 };
	bool isValid = false;

	std::vector<Tile> neighbourTiles;
	std::vector<sf::Vector2i> validMovePositions;

	allPiecesOnBoard = FindPiecesOnBoard();


	for each (Tile validTile in allPiecesOnBoard)
	{
		sf::Vector2i tileOnBoardPos = GetCellPosFromTile(validTile);
		neighbourTiles = GetAiNeighbours(tileOnBoardPos.x, tileOnBoardPos.y);

		for each (Tile tileToPlace in neighbourTiles)
		{
			if (tileToPlace.GetCurrentColor() == EMPTY_C)
			{
				// GETTING THE CELL POS OF THE TILE YOU WANT TO PLACE
				movePos = GetCellPosFromTile(tileToPlace);

				currentCellPos = movePos;

				
				isValid = CheckValidTileOrShapeAi(validTile);
				

				if (isValid)
				{
					isValid = CheckFurtherInLine(aiTiles[aiSelectedTile], validTile);
				}

				Tile selectedTileTile = aiTiles[aiSelectedTile];
				sf::Vector2i tileWeAreCheckingAgainst = GetCellPosFromTile(validTile);
				sf::Vector2i neighbourTemp = GetCellPosFromTile(tileToPlace);

				

				if (isValid && movesInTurnCount != 0)
				{
					if (movePos.x == CheckIfPlacingInSameLine(movePos.x, movePos.y) || movePos.y == CheckIfPlacingInSameLine(movePos.x, movePos.y))
					{
						isValid = true;
					}
					else
					{
						isValid = false;
					}
				}

				if (isValid)
				{
					movePos = GetCellPosFromTile(tileToPlace);
					validMovePositions.push_back(movePos);
					bool x = CheckFurtherInLine(aiTiles[aiSelectedTile], validTile);
					bool y = CheckValidTileOrShapeAi(validTile);

					if (movesInTurnCount == 0)
					{
						sameLineVector = { movePos.x, movePos.y };
					}

					return movePos;
				}
				else
				{
					movePos = { -1, -1 };
				}
			}
		}
	}
	return movePos;
}

std::vector<Tile> Game::FindPiecesOnBoard()
{
	// REMOVE ALL TILES FROM VECTOR AT START
	allPiecesOnBoard.clear();

	std::vector<Tile> tilesOnBoard;
	for (int row = 0; row < Global::ROWS_COLUMNS; row++)
	{
		for (int col = 0; col < Global::ROWS_COLUMNS; col++)
		{
			if (board[row][col].GetCurrentColor() != EMPTY_C)
			{
				// ADDS ALL PIECES ON BOARD TO THE LIST
				tilesOnBoard.push_back(board[row][col]);
			}
		}

	}
	return tilesOnBoard;
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

bool Game::CheckValidTileOrShapeAi(Tile tile)
{
	bool isValid = false;

	for (int i = 0; i < 6; i++)
	{
		if (!aiTiles[i].GetUsed())
		{
			if (aiTiles[i].GetCurrentColor() == tile.GetCurrentColor() ||
				aiTiles[i].GetCurrentShape() == tile.GetCurrentShape())
			{
				aiCurrentPiece = aiTiles[i].GetCurrentPiece();
				aiSelectedTile = i;
				isValid = true;
				break;
			}
		}
	}

	sf::Vector2i temp = GetCellPosFromTile(tile);
	std::vector<Tile> neighbourTiles = GetAiNeighbours(currentCellPos.x, currentCellPos.y);

	for each (Tile tile in neighbourTiles)
	{
		if (tile.GetCurrentColor() != EMPTY_C)
		{
			if (aiTiles[aiSelectedTile].GetCurrentColor() != tile.GetCurrentColor() &&
				aiTiles[aiSelectedTile].GetCurrentShape() != tile.GetCurrentShape())

			{
				isValid = false;
				return isValid;
			}
		}
		
	}

	return isValid;
}

std::vector<Tile> Game::GetValidNieghboursAi(std::vector<Tile> neighbours)
{
	std::vector<Tile> validNeighbourTiles;

	for each (Tile tile in neighbours)
	{
		if (tile.GetCurrentColor() != EMPTY_C || tile.GetCurrentShape() != EMPTY_S)
		{
			validNeighbourTiles.push_back(tile);
		}
	}

	return validNeighbourTiles;
}

sf::Vector2i Game::MakeAiMove()
{
	sf::Vector2i movePos;

	movePos = GetAiMove();
	//aiTiles[aiSelectedTile]



	return movePos;
}




void Game::AiPlaceTileOnBoard(sf::Vector2i boardPos)
{
	currentCellPos = boardPos;
	int row = boardPos.x;
	int col = boardPos.y;

	//
	// Do more stuff,

	if (isFirstMoveOfGame)
	{
		aiCurrentPiece = aiTiles[0].GetCurrentPiece();
		board[row][col].SetPiece(aiCurrentPiece);
	}
	else if (!aiTiles[aiSelectedTile].GetUsed())
	{
		board[row][col].SetPiece(aiCurrentPiece);
	}
	std::cout << "AI has placed a " + board[row][col].tileName + " on tile " + std::to_string(row) + ", " + std::to_string(col) + "\n";
	aiTiles[aiSelectedTile].SetUsed();

}

//int MiniMax(int n)
//{
//	int score = 0;
//
//	if N is deep enough then
//		return the estimated score of this leaf
//	else
//		Let N1, N2, .., Nm be the successors of N;
//	if N is a Min node then
//		return min{ MINIMAX(N1), .., MINIMAX(Nm) }
//	else
//		return max{ MINIMAX(N1), .., MINIMAX(Nm) }
//	return score;
//}

