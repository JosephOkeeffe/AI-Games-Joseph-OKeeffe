
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
		TileCount();
	}
	if (sf::Keyboard::Num1 == t_event.key.code)
	{
		OutputTilePool();
	}
	if (sf::Keyboard::Num2 == t_event.key.code)
	{
		OutputPlayerTile();
	}
	if (sf::Keyboard::Num3 == t_event.key.code)
	{
		OutputAITiles();
	}
	if (sf::Keyboard::B == t_event.key.code)
	{
		isBagOpen = !isBagOpen;
	}
}

void Game::ProcessMouseDown(sf::Event t_event)
{
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(Global::GetMousePos(m_window));

	

	if (sf::Mouse::Left == t_event.key.code)
	{

		// PLAYER TILES
		for (int i = 0; i < 6; i++)
		{
			sf::FloatRect bounds = playerTiles[i].shape.getGlobalBounds();

			if (bounds.contains(mousePos))
			{
				// Delsecect old tile
				playerTiles[selectedTile].DeselectTile();
				// Select new tile
				playerTiles[i].SelectTile();
				// Get a new selected tile
				selectedTile = i;
				// Get the shape / piece from the tile you selected
				selectedPiece = playerTiles[i].GetCurrentPiece();
			}
		}

		//  BOARD
		for (int row = 0; row < Global::ROWS_COLUMNS; row++)
		{
			for (int col = 0; col < Global::ROWS_COLUMNS; col++)
			{
				sf::FloatRect bounds = board[row][col].shape.getGlobalBounds();

				if (bounds.contains(mousePos))
				{
					if (playerTiles[selectedTile].isSelected && !board[row][col].isPlaced)
					{
						// Change piece on board to the same value as the selected piece
						board[row][col].SetPiece(selectedPiece);
						// selectedPiece = NONE;
						// Update piece on board
						board[row][col].CheckPiece();
						// Deselecting the tile
						playerTiles[selectedTile].DeselectTile();
						// Getting random number from the active pool
						int randomTileFromTilePool = GetActiveTilePool();
						// Refilling the players tile with the random piece
						playerTiles[selectedTile].SetPiece(randomTileFromTilePool);
						// Removing the tile from the tile pool
						totalTilePool[randomTileFromTilePool].SetUsed();
						//playerTiles[selectedTile].SetUsed();

						std::cout << "You have placed : " << board[row][col].tileName << "\n";

					}
				}
			}
		}
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

