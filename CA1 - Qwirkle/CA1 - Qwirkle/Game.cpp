
#include "Game.h"
#include <iostream>

Game::Game() :
	m_window{ sf::VideoMode{ Global::S_WIDTH, Global::S_HEIGHT + 100, 32U }, "Qwirkle" },
	m_exitGame{false}
{
	srand(time(nullptr));
	initTiles();

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
		for (int row = 0; row < Global::ROWS_COLUMNS; row++)
		{
			for (int col = 0; col < Global::ROWS_COLUMNS; col++)
			{
				tiles[row][col].currentPiece = Piece::CP;
				tiles[row][col].CheckPiece();
			}
		}
	}
}

void Game::ProcessMouseDown(sf::Event t_event)
{
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(Global::GetMousePos(m_window));

	if (sf::Mouse::Left == t_event.key.code)
	{
		for (int col = 0; col < 6; col++)
		{
			sf::FloatRect bounds = playerTiles[0][col].shape.getGlobalBounds();

			if (bounds.contains(mousePos))
			{
				playerTiles[0][col].tile.setFillColor(sf::Color::Yellow);
				selectedPiece = playerTiles[0][col].currentPiece;
			}
		}

		for (int row = 0; row < Global::ROWS_COLUMNS; row++)
		{
			for (int col = 0; col < Global::ROWS_COLUMNS; col++)
			{
				sf::FloatRect bounds = tiles[row][col].shape.getGlobalBounds();

				if (bounds.contains(mousePos))
				{
					tiles[row][col].currentPiece = selectedPiece;
					selectedPiece = NONE;
					tiles[row][col].CheckPiece();
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
			tiles[row][col].Render(m_window);
		}
	}

	for (int col = 0; col < 6; col++)
	{
		playerTiles[0][col].Render(m_window);
	}

	for (int col = 0; col < 6; col++)
	{
		aiTiles[0][col].Render(m_window);
	}
	m_window.display();
}

void Game::initTiles()
{
	tiles = new Tile * [Global::ROWS_COLUMNS];

	for (int i = 0; i < Global::ROWS_COLUMNS; i++)
	{
		tiles[i] = new Tile[Global::ROWS_COLUMNS];

	}

	for (int row = 0; row < Global::ROWS_COLUMNS; row++)
	{
		for (int col = 0; col < Global::ROWS_COLUMNS; col++)
		{
			tiles[row][col].Init(sf::Vector2f(row * Global::CELL_SIZE, col * Global::CELL_SIZE));
		}
	}

	playerTiles = new Tile * [1];

	for (int i = 0; i < 1; i++)
	{
		playerTiles[i] = new Tile[6];

	}

	for (int col = 0; col < 6; col++)
	{
		playerTiles[0][col].Init(sf::Vector2f((col * Global::CELL_SIZE) + 75, 1520)); 
		playerTiles[0][col].SetRandomTile();
	}

	aiTiles = new Tile * [1];

	for (int i = 0; i < 1; i++)
	{
		aiTiles[i] = new Tile[6];

	}

	for (int col = 0; col < 6; col++)
	{
		aiTiles[0][col].Init(sf::Vector2f((col * Global::CELL_SIZE) + Global::S_WIDTH - 525, 1520));
		aiTiles[0][col].SetRandomTile();
	}
}


