
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
	if (sf::Keyboard::Num2 == t_event.key.code)
	{
		ShufflePlayerTiles();
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

	playerScoreText.setString(std::to_string(playerScore));
	processEvents();
	SortAiTurn();
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

				isValidPlacement = CheckValidTileColorOrShape(CheckValidNeighbours(row, col));
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
					if (movesInTurnCount == 0 && isFirstMoveOfGame)
					{
						firstTilePlacedInTurn = board[row][col];
					}
					if(movesInTurnCount >= 1 && currentTurn == 1)
					{
						firstTilePlacedInTurn = board[row][col];
					}

					if (movesInTurnCount == 0)
					{
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
			return -1;
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
	//SameLineVector.clear();
	//firstMoveInTurn = true;
	//linesPlacedInTurn.clear();
	if(isPlayerTurn){ playerScore += AddScoresForTurn(); }
	
	movesInTurnCount = 0;
	isColorTurn = false;
	isShapeTurn = false;
	turnColor = EMPTY_C;
	turnShape = EMPTY_S;
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
	sf::Vector2i playerCell;
	playerCell.x = currentCellPos.x;
	playerCell.y = currentCellPos.y;

	sf::Vector2i validTileCell = GetTileCell(validTile);

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
		_Output_To_Screen("Tiles are on the same COLUMN");

		if (validTileCell.y < playerCell.y)
		{
			// CHECK IN A LINE GOING UP, IF THE COLORS MATCH. STOPS WHEN IT FINDS AN EMPTY TILE
			for (int i = 0; validTileCell.y - i >= 0 && board[validTileCell.x][validTileCell.y - i].GetCurrentColor() != EMPTY_C; i++)
			{
				//validTileCell.y = (validTileCell.y - i < 0) ? 0 : validTileCell.y;
				int tilesInLineCount = validTileCell.y - i;
				
				// CANT HAVE THE EXACT SAME TILE IN THE SAME LINE
				if (board[validTileCell.x][tilesInLineCount].GetCurrentPiece() == playerTile.GetCurrentPiece())
				{
					_Output_To_Screen("Cant place two of the same tiles in the same line");

					isValid = false;
					return isValid;
				}
				else
				{
					if (board[validTileCell.x][tilesInLineCount].GetCurrentColor() == playerTile.GetCurrentColor())
					{
						_Output_To_Screen("Tile color matches the tile ABOVE");
						isValid = true;
						tilesInCurrentLineForScore[i] = (board[validTileCell.x][tilesInLineCount]);
					}
					else if (board[validTileCell.x][tilesInLineCount].GetCurrentShape() == playerTile.GetCurrentShape())
					{
						_Output_To_Screen(" Tile shape matches the tile ABOVE");
						isValid = true;
						tilesInCurrentLineForScore[i] = (board[validTileCell.x][tilesInLineCount]);
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
					_Output_To_Screen("Cant place two of the same tiles in the same line");

					isValid = false;
					return isValid;
				}
				else
				{
					if (board[validTileCell.x][validTileCell.y + i].GetCurrentColor() == playerTile.GetCurrentColor())
					{
						_Output_To_Screen("Tile color matches the tile BELOW");
						isValid = true;
						tilesInCurrentLineForScore[i] = (board[validTileCell.x][validTileCell.y + i]);
					}
					else if (board[validTileCell.x][validTileCell.y + i].GetCurrentShape() == playerTile.GetCurrentShape())
					{
						_Output_To_Screen(" Tile shape matches the tile BELOW");
						isValid = true;
						tilesInCurrentLineForScore[i] = (board[validTileCell.x][validTileCell.y + i]);
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
		_Output_To_Screen("Tiles are on the same ROW");
		if (validTileCell.x < playerCell.x)
		{
			// CHECK IN A LINE GOING UP, IF THE COLORS MATCH. STOPS WHEN IT FINDS AN EMPTY TILE
			for (int i = 0; validTileCell.x - i >= 0 && board[validTileCell.x - i][validTileCell.y].GetCurrentColor() != EMPTY_C; i++)
			{
				// CANT HAVE THE EXACT SAME TILE IN THE SAME LINE
				if (board[validTileCell.x - i][validTileCell.y].GetCurrentPiece() == playerTile.GetCurrentPiece())
				{
					_Output_To_Screen("Cant place two of the same tiles in the same line");

					isValid = false;
					return isValid;
				}
				else
				{
					if (board[validTileCell.x - i][validTileCell.y].GetCurrentColor() == playerTile.GetCurrentColor())
					{
						_Output_To_Screen("Tile color matches the tile to the LEFT");
						isValid = true;
						tilesInCurrentLineForScore[i] = (board[validTileCell.x - i][validTileCell.y]);
					}
					else if (board[validTileCell.x - i][validTileCell.y].GetCurrentShape() == playerTile.GetCurrentShape())
					{
						_Output_To_Screen(" Tile shape matches the tile to the RIGHT");
						isValid = true;
						tilesInCurrentLineForScore[i] = (board[validTileCell.x - i][validTileCell.y]);
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
					_Output_To_Screen("Cant place two of the same tiles in the same line");

					isValid = false;
					return isValid;
				}
				else
				{
					if (board[validTileCell.x + i][validTileCell.y].GetCurrentColor() == playerTile.GetCurrentColor())
					{
						_Output_To_Screen("Tile color matches the tile to the LEFT");
						isValid = true;
						tilesInCurrentLineForScore[i] = (board[validTileCell.x + i][validTileCell.y]);
					}
					else if (board[validTileCell.x + i][validTileCell.y].GetCurrentShape() == playerTile.GetCurrentShape())
					{
						_Output_To_Screen(" Tile shape matches the tile to the RIGHT");
						isValid = true;
						tilesInCurrentLineForScore[i] = (board[validTileCell.x + i][validTileCell.y]);
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

///////////////
///////////////
///////////////

void Game::SortAiTurn()
{	
	if (!isPlayerTurn) // AI  Turn
	{
		if (aiTimer.asSeconds() == 0) { aiClock.restart();}
		aiTimer = aiClock.getElapsedTime();
		sf::Vector2i move = MakeAiMove();

		if (isFirstMoveOfGame)
		{
			move = { rand() % Global::ROWS_COLUMNS,rand() % Global::ROWS_COLUMNS};
			
		}

		if (move.x == -1)
		{
			_Output_To_Screen("AI has no valid move: skipping turn")
			NextTurn();
			return;
		}
		else{AiPlaceTileOnBoard(move);}
		// 
		if (aiTimer.asSeconds() >= 2)
		{
			_Output_To_Screen("Players Turn!")
			NextTurn();
		}

		isFirstMoveOfGame = false;

	}
}

sf::Vector2i Game::GetAiMove()
{
	sf::Vector2i movePos = { -1, -1 };
	sf::Vector2i foundPiece;
	std::vector<Tile> neighbourTiles;

	for (int row = 0; row < Global::ROWS_COLUMNS; row++)
	{
		for (int col = 0; col < Global::ROWS_COLUMNS; col++)
		{
			// FINDS FIRST PIECE ON BOARD THAT ISNT BLANK
			if (board[row][col].GetCurrentColor() != EMPTY_C)
			{
				foundPiece = { row, col };
				// GETS NEIGHBOURS OF THE FIRST PIECE IT FINDS
				neighbourTiles = GetAiNeighbours(row, col);
				// CHECKS TO SEE IF THEY ARE EMPTY SO A TILE CAN BE PLACED IN IT
				for each (Tile neighbour in neighbourTiles)
				{
					if (neighbour.GetCurrentColor() == EMPTY_C)
					{
						sf::Vector2i neigbourPos = GetTileCell(neighbour);

						if (CheckValidTileOrShapeAi(GetAiNeighbours(neigbourPos.x, neigbourPos.y)))
						{
							movePos = GetTileCell(neighbour);
							break;
						}
					}
				}
			}
		}

	}
	return movePos;
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

bool Game::CheckValidTileOrShapeAi(std::vector<Tile> neighbours)
{

	for each (Tile neighbour in neighbours)
	{
		for (int i = 0; i < 6; i++)
		{
			if (aiTiles[i].GetCurrentColor() == neighbour.GetCurrentColor())
			{
				aiCurrentPiece = aiTiles[i].GetCurrentPiece();
				aiSelectedTile = i;
				return true;
			}
			else if (aiTiles[i].GetCurrentShape() == neighbour.GetCurrentShape())
			{
				aiCurrentPiece = aiTiles[i].GetCurrentPiece();
				aiSelectedTile = i;
				return true;
			}
		}
	}
	return false;
}

int Game::AddScoresForTurn()
{
	int score = 1;

	for each (Tile tile in tilesInCurrentLineForScore)
	{
		if (tile.GetCurrentColor() != EMPTY_C)
		{
			score++;
		}
	}

	return score;
}

sf::Vector2i Game::MakeAiMove()
{
	sf::Vector2i movePos;

	movePos = GetAiMove();



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
	aiTiles[aiSelectedTile].SetUsed();

}



