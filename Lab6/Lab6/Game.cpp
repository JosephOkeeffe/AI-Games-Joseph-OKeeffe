#include "Game.h"
#include <iostream>
#include "Global.h"
#include <queue>

Game::Game() :
	m_window{ sf::VideoMode{ Global::S_WIDTH, Global::S_HEIGHT, 32U }, "SFML Game" },
	m_exitGame{false}
{
	init();
	setupSprite();
}

Game::~Game()
{
}

void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps);
	while (m_window.isOpen())
	{
		processEvents();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents();
			update(timePerFrame);
		}
		render();
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
		if (sf::Event::MouseButtonReleased == newEvent.type)
		{
			processMouse(newEvent);
		}
	}
}

void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
}
void Game::processMouse(sf::Event t_event)
{



	if (sf::Mouse::Left == t_event.key.code)
	{
		

		if (GetCurrentCell().x == goalPos.x && GetCurrentCell().y == goalPos.y && isGoalTile) { return; }

		previousCellPos = currentCellPos;
		currentCellPos = GetCurrentCell();
		
		if (isStartTile)
		{
			tiles[previousCellPos.x][previousCellPos.y].Reset();
			tiles[GetCurrentCell().x][GetCurrentCell().y].SetStart();
		}
		else
		{
			isStartTile = true;
			tiles[currentCellPos.x][currentCellPos.y].SetStart();
		}
		startPos = currentCellPos;
		
	}

	if (sf::Mouse::Middle == t_event.key.code)
	{
		tiles[GetCurrentCell().x][GetCurrentCell().y].SetObstacle();
	}
	if (sf::Mouse::Right == t_event.key.code)
	{
		if (!isGoalTile)
		{
			isGoalTile = true;
			goalPos = GetCurrentCell();
			tiles[GetCurrentCell().x][GetCurrentCell().y].SetGoal();

			BushFire();
		}
	}

}

void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame){m_window.close();}
	if (!isStartTile || !isGoalTile) { return; }
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		std::vector<sf::Vector2i> path = FindPath();

		//if (!path.empty()) 
		//{
		//	for (const sf::Vector2i& position : path) 
		//	{
		//		if (position.x >= 0 && position.x < Global::ROWS_COLUMNS &&
		//			position.y >= 0 && position.y < Global::ROWS_COLUMNS) 
		//		{
		//			Tile& pathTile = tile[position.x][position.y];
		//			// Set the color of the path tile to yellow
		//			pathTile.tile.setFillColor(sf::Color::Yellow);
		//		}
		//	}
		//}
		//else {
		//	// Handle the case where no valid path is found from start to goal.
		//}
	}
}

void Game::init()
{
	font.loadFromFile("./ASSETS/FONTS/ariblk.ttf");
	
	
	tiles = new Tile * [Global::ROWS_COLUMNS];

	for (int i = 0; i < Global::ROWS_COLUMNS; i++)
	{
		tiles[i] = new Tile[Global::ROWS_COLUMNS];
		
	}
	initTiles();
}

void Game::render()
{
	m_window.clear(sf::Color::Black);

	for (int row = 0; row < Global::ROWS_COLUMNS; row++)
	{
		for (int col = 0; col < Global::ROWS_COLUMNS; col++)
		{
			tiles[row][col].Render(m_window);
		}
	}
	m_window.display();

}

void Game::initTiles()
{
	for (int row = 0; row < Global::ROWS_COLUMNS; row++)
	{
		for (int col = 0; col < Global::ROWS_COLUMNS; col++)
		{
			tiles[row][col].Init(sf::Vector2f(row * Global::CELL_SIZE, col * Global::CELL_SIZE), font);
		}
	}
}

sf::Vector2i Game::GetCurrentCell()
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
	mousePos /= Global::CELL_SIZE;

	std::cout << "Cell X:" << mousePos.x << "\n";
	std::cout << "Cell Y:" << mousePos.y << "\n";
	return mousePos;
}

void Game::setupSprite()
{
}



void Game::BushFire()
{
    for (int x = 0; x < Global::ROWS_COLUMNS; x++)
    {
        for (int y = 0; y < Global::ROWS_COLUMNS; y++)
        {
            tiles[x][y].integrationField = INT_MAX;
        }
    }

    std::queue<sf::Vector2i> queue;
    queue.push(goalPos);

	tiles[goalPos.x][goalPos.y].cost = 0;
    tiles[goalPos.x][goalPos.y].integrationField = 0;

    while (!queue.empty())
    {
        sf::Vector2i currentPos = queue.front();
        queue.pop();
        Tile& currentTile = tiles[currentPos.x][currentPos.y];

        for (int rows = -1; rows <= 1; rows++)
        {
            for (int cols = -1; cols <= 1; cols++)
            {
                int newX = currentPos.x + rows;
                int newY = currentPos.y + cols;

                if (newX >= 0 && newX < Global::ROWS_COLUMNS 
				&& newY >= 0 && newY < Global::ROWS_COLUMNS)
                {
                    Tile& neighborTile = tiles[newX][newY];
					neighborTile.UpdateCost();

                    int newIntegrationField = currentTile.integrationField + 1;
                    if (newIntegrationField < neighborTile.integrationField)
                    {
                        neighborTile.integrationField = newIntegrationField;
                        neighborTile.floField = sf::Vector2f(rows, cols);

                        neighborTile.cost = newIntegrationField;

                        queue.push(sf::Vector2i(newX, newY));
                    }
                }
            }
        }
    }
}

std::vector<sf::Vector2i> Game::FindPath()
{
	std::vector<sf::Vector2i> path;

	if (startPos.x < 0 || startPos.x >= Global::ROWS_COLUMNS || startPos.y < 0 || startPos.y >= Global::ROWS_COLUMNS)
	{
		return path;
	}

	//// Start from the given start position
	//sf::Vector2i currentPos = startPos;

	//while (currentPos != goalPos)
	//{
	//	path.push_back(currentPos);

	//	Tile& currentTile = tile[currentPos.x][currentPos.y];
	//	sf::Vector2f flowDirection = currentTile.floField;
	//	currentTile.tile.setFillColor(sf::Color::Yellow);
	//	// Update the next position based on the flow direction
	//	currentPos.x += static_cast<int>(flowDirection.x);
	//	currentPos.y += static_cast<int>(flowDirection.y);
	//}

	//// Add the goal position to the path
	//path.push_back(goalPos);

	return path;
}






