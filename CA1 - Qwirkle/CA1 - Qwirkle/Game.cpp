
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
}

void Game::ProcessMouseDown(sf::Event t_event)
{
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(Global::GetMousePos(m_window));

	

	if (sf::Mouse::Left == t_event.key.code)
	{
		for (int i = 0; i < 6; i++)
		{
			sf::FloatRect bounds = playerTiles[i].shape.getGlobalBounds();

			if (bounds.contains(mousePos))
			{
				playerTiles[selected].Reset();
				playerTiles[i].SelectTile();
				selected = i;
				selectedPiece = playerTiles[i].GetPiece();
			}
		}

		for (int row = 0; row < Global::ROWS_COLUMNS; row++)
		{
			for (int col = 0; col < Global::ROWS_COLUMNS; col++)
			{
				sf::FloatRect bounds = board[row][col].shape.getGlobalBounds();

				if (bounds.contains(mousePos))
				{
					board[row][col].SetPiece(selectedPiece);
					selectedPiece = NONE;
					// Update piece on board
					board[row][col].CheckPiece();
					//
					playerTiles[selected].SetUsed();
					playerTiles[selected].SetPiece(GetActiveTilePool());
					//playerTiles[selected].Reset();
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

	for (int i = 0; i < 6; i++) // Draw only 6
	{
		playerTiles[i].Render(m_window);
	}

	//for (int i = 0; i < 6; i++) // 
	//{
	//	aiTiles[i].Render(m_window);
	//}
	m_window.display();
}

void Game::init()
{
	InitBoard();
	SetupTilePool();
	SetupPlayerTiles();
	SetupAITiles();
}

void Game::InitBoard()
{
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
		tilePool1[i].Init(sf::Vector2f(999, 999));
		tilePool1[i].SetPiece(i);
	}
	for (int i = 0; i < 36; i++)
	{
		tilePool2[i].Init(sf::Vector2f(999, 999));
		tilePool2[i].SetPiece(i);
	}
	for (int i = 0; i < 36; i++)
	{
		tilePool3[i].Init(sf::Vector2f(999, 999));
		tilePool3[i].SetPiece(i);
	}

	std::copy(tilePool1, tilePool1 + 36, totalTilePool);
	std::copy(tilePool2, tilePool2 + 36, totalTilePool + 36);
	std::copy(tilePool3, tilePool3 + 36, totalTilePool + 72);

	std::shuffle(totalTilePool, totalTilePool + 108, std::default_random_engine());

}

void Game::SetupPlayerTiles()
{
	for (int i = 0; i < 6; i++)
	{
		playerTiles[i].Init(sf::Vector2f((i * Global::CELL_SIZE) + 75, 1520));
		if (!totalTilePool[i].GetUsed())
		{
			playerTiles[i].SetPiece(totalTilePool[i].GetPiece());
			totalTilePool[i].SetUsed();
		}
	}
}

void Game::SetupAITiles()
{
	//for (int i = 7; i < 12; i++)
	//{
	//	aiTiles[i].Init(sf::Vector2f((i * Global::CELL_SIZE) + Global::S_WIDTH - 525, 1520));
	//	if (!totalTilePool[i].GetUsed())
	//	{
	//		aiTiles[i].SetPiece(totalTilePool[i].GetPiece()); // Start from index 6 to avoid duplicates
	//		totalTilePool[i].SetUsed();
	//	}
	//}
}

int Game::GetActiveTilePool()
{
	int random = rand() % 108;

	if (!totalTilePool[random].isUsed)
	{
		totalTilePool[random].SetUsed();
		return random;
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


