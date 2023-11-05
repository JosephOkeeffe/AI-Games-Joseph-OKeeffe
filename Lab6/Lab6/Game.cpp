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
	if (sf::Keyboard::Num1 == t_event.key.code)
	{
		for (int row = 0; row < Global::ROWS_COLUMNS; row++)
		{
			for (int col = 0; col < Global::ROWS_COLUMNS; col++)
			{
				tiles[row][col].drawCost = !tiles[row][col].drawCost;
			}
		}
	}
	if (sf::Keyboard::Space == t_event.key.code)
	{
		//FindPath();
		/*for each (sf::Vector2i position in  FindPath())
		{
			tiles[position.x][position.y];
			tiles[position.x][position.y].tile.setFillColor(sf::Color::Yellow);
		}*/
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
		UpdateText();
		
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
			UpdateText();
		
			//BushFire();
		}
	}

}

void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame){m_window.close();}
	if (!isStartTile || !isGoalTile) { return; }

}

void Game::init()
{
	font.loadFromFile("./ASSETS/FONTS/ariblk.ttf");

	UpdateText();

	startPosText.setFont(font);
	startPosText.setFillColor(sf::Color::Green);
	startPosText.setCharacterSize(35);
	startPosText.setPosition(20, Global::S_HEIGHT * 0.2);
	startPosText.setOutlineThickness(1);
	startPosText.setOutlineColor(sf::Color::Black);

	goalPosText.setFont(font);
	goalPosText.setFillColor(sf::Color::Red);
	goalPosText.setCharacterSize(35);
	goalPosText.setPosition(Global::S_WIDTH - (goalPosText.getLocalBounds().width * 1.1), Global::S_HEIGHT * 0.2);
	goalPosText.setOutlineThickness(1);
	goalPosText.setOutlineColor(sf::Color::Black);

	distanceText.setFont(font);
	distanceText.setFillColor(sf::Color::Yellow);
	distanceText.setCharacterSize(35);
	distanceText.setPosition((Global::S_WIDTH / 2) - (distanceText.getLocalBounds().width / 2), Global::S_HEIGHT * 0.2);
	distanceText.setOutlineThickness(1);
	distanceText.setOutlineColor(sf::Color::Black);


	
	
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
			tiles[row][col].line[0] = tiles[row][col].tile.getPosition() + sf::Vector2f(Global::CELL_SIZE / 2, Global::CELL_SIZE / 2);
			tiles[row][col].line[1] = tiles[row][col].tile.getPosition() + sf::Vector2f(Global::CELL_SIZE / 2, Global::CELL_SIZE / 2) + tiles[row][col].floField * 20.0f;
			tiles[row][col].line[0].color = sf::Color::Black;
			tiles[row][col].line[1].color = sf::Color::White;
			tiles[row][col].Render(m_window);
		}
	}

	m_window.draw(startPosText);
	m_window.draw(goalPosText);
	m_window.draw(distanceText);
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
void Game::UpdateText()
{
	startPosText.setString("Start Pos: (" + std::to_string(startPos.x) + ", " + std::to_string(startPos.y) + ")");
	goalPosText.setString("Goal Pos: (" + std::to_string(goalPos.x) + ", " + std::to_string(goalPos.y) + ")");
	int distanceX = std::abs(startPos.x - goalPos.x);
	int distanceY = std::abs(startPos.y - goalPos.y);
	distanceText.setString("Distance: (" + std::to_string(distanceX) + ", " + std::to_string(distanceY) + ")");

	goalPosText.setPosition(Global::S_WIDTH - (goalPosText.getLocalBounds().width * 1.1), Global::S_HEIGHT * 0.2);

}

sf::Vector2i Game::GetCurrentCell()
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
	mousePos /= Global::CELL_SIZE;
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

	// The goal is set to 0
	tiles[goalPos.x][goalPos.y].cost = 0;
	tiles[goalPos.x][goalPos.y].integrationField = 0;

	while (!queue.empty())
	{
		sf::Vector2i currentPos = queue.front();
		queue.pop();
		// Getting the current tile
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
					// Getting neighbour tile
					Tile& neighbourTile = tiles[newX][newY];
					neighbourTile.UpdateCost();

					// Getting the int field
					int newIntegrationField = currentTile.integrationField + 1; // distance + cost
					
					if (newIntegrationField < neighbourTile.integrationField)
					{
						neighbourTile.integrationField = newIntegrationField;
						neighbourTile.cost = newIntegrationField;

						if (!neighbourTile.isObstacleTile)
						{
							neighbourTile.UpdateCost();
							neighbourTile.cost = newIntegrationField;

							// Calculate the new floField for neighborTile.
							/*neighbourTile.floField = sf::Vector2f(goalPos.x - newX, goalPos.y - newY);
							float length = std::sqrt(neighbourTile.floField.x * neighbourTile.floField.x + neighbourTile.floField.y * neighbourTile.floField.y);

							if (length > 0)
							{
								neighbourTile.floField /= length;
							}*/

							queue.push(sf::Vector2i(newX, newY));
						}
						else
						{
							neighbourTile.cost = 999;
						}

					}
				}
			}
		}
	}
}

void Game::CalculateFloField()
{
	
	//neighborTile.floField = sf::Vector2f(goalPos.x - newX, goalPos.y - newY);

	//float length = std::sqrt(neighborTile.floField.x * neighborTile.floField.x + neighborTile.floField.y * neighborTile.floField.y);
	//if (length > 0)
	//{
	//	neighborTile.floField /= length;
	//}
	
}


std::vector<sf::Vector2i> Game::FindPath()
{
	if (!isStartTile || !isGoalTile)
	{
		return path; 
	}


	sf::Vector2i currentPosition = startPos;

	while (currentPosition != goalPos)
	{
		Tile& currentTile = tiles[currentPosition.x][currentPosition.y];
		sf::Vector2f direction = currentTile.floField;

		sf::Vector2i nextPosition(static_cast<int>(currentPosition.x + direction.x), static_cast<int>(currentPosition.y + direction.y));


		if (nextPosition.x >= 0 && nextPosition.x < Global::ROWS_COLUMNS &&
			nextPosition.y >= 0 && nextPosition.y < Global::ROWS_COLUMNS)
		{
			std::cout << "Path X: " << currentPosition.x << "\n";
			std::cout << "Path Y: " << currentPosition.y << "\n";
			tiles[currentPosition.x][currentPosition.y].tile.setFillColor(sf::Color::Yellow);
			path.push_back(currentPosition);
			currentPosition = nextPosition;
		}
		else
		{
			path.clear();
			break;
		}
	}

	return path;
}







