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
		std::cout << "Displaying Cost \n";

		for (int row = 0; row < Global::ROWS_COLUMNS; row++)
		{
			for (int col = 0; col < Global::ROWS_COLUMNS; col++)
			{
				tiles[row][col].TurnOnCost();

			}
		}
	}
	if (sf::Keyboard::Num2 == t_event.key.code)
	{
		std::cout << "Displaying Integration \n";
		for (int row = 0; row < Global::ROWS_COLUMNS; row++)
		{
			for (int col = 0; col < Global::ROWS_COLUMNS; col++)
			{
				tiles[row][col].TurnOnIntegration();
				

			}
		}
	}
	if (sf::Keyboard::Num3 == t_event.key.code)
	{
		std::cout << "Displaying Lines \n";

		for (int row = 0; row < Global::ROWS_COLUMNS; row++)
		{
			for (int col = 0; col < Global::ROWS_COLUMNS; col++)
			{
				tiles[row][col].TurnOnLines();

			}
		}
	}
	if (sf::Keyboard::Space == t_event.key.code)
	{
		FindPath();
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
		
			BushFire();
			CalculateFloField();
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
	startPosText.setPosition(20, Global::S_HEIGHT * 0.01);
	startPosText.setOutlineThickness(1);
	startPosText.setOutlineColor(sf::Color::Black);

	goalPosText.setFont(font);
	goalPosText.setFillColor(sf::Color::Red);
	goalPosText.setCharacterSize(35);
	goalPosText.setPosition(Global::S_WIDTH - (goalPosText.getLocalBounds().width * 1.2), Global::S_HEIGHT * 0.01);
	goalPosText.setOutlineThickness(1);
	goalPosText.setOutlineColor(sf::Color::Black);

	distanceText.setFont(font);
	distanceText.setFillColor(sf::Color::Yellow);
	distanceText.setCharacterSize(35);
	distanceText.setPosition((Global::S_WIDTH / 2) - (distanceText.getLocalBounds().width / 2), Global::S_HEIGHT * 0.01);
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
	m_window.clear(sf::Color::Blue);

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

	//goalPosText.setPosition(Global::S_WIDTH - (goalPosText.getLocalBounds().width * 1.1), Global::S_HEIGHT * 0.2);
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
			tiles[x][y].cost = INT_MAX;
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
					neighbourTile.UpdateTextOnScreen();

					int newCost = currentTile.cost + 1; 

					if (newCost < neighbourTile.cost)
					{
						neighbourTile.cost = newCost;

						if (!neighbourTile.isObstacleTile)
						{
							neighbourTile.UpdateTextOnScreen();

							// Calculate direct distance to the goal using actual vector positions
							sf::Vector2f goalPosition = tiles[goalPos.x][goalPos.y].tile.getPosition();
							sf::Vector2f currentPosition = tiles[currentPos.x][currentPos.y].tile.getPosition();

							int dx = std::abs(currentPosition.x - goalPosition.x);
							int dy = std::abs(currentPosition.y - goalPosition.y);

					/*		int dx = std::abs(newX - goalPos.x);
							int dy = std::abs(newY - goalPos.y);*/
							int directDistance = dx + dy;

							// Calculate integrationField
							neighbourTile.integrationField = directDistance + neighbourTile.cost;

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
	if (!tiles[goalPos.x][goalPos.y].isGoalTile)
	{
		return;
	}

	for (int x = 0; x < Global::ROWS_COLUMNS; x++)
	{
		for (int y = 0; y < Global::ROWS_COLUMNS; y++)
		{
			Tile& currentTile = tiles[x][y];
			sf::Vector2f floField(0.0f, 0.0f); // Initialize the vector field to (0,0).

			if (!currentTile.isObstacleTile)
			{
				int lowestIntegrationField = currentTile.integrationField;

				// Iterate through the neighbors to find the lowest integration field.
				for (int rows = -1; rows <= 1; rows++)
				{
					for (int cols = -1; cols <= 1; cols++)
					{
						int newX = x + rows;
						int newY = y + cols;

						if (newX >= 0 && newX < Global::ROWS_COLUMNS && newY >= 0 && newY < Global::ROWS_COLUMNS)
						{
							const Tile& neighborTile = tiles[newX][newY];
							if (neighborTile.integrationField < lowestIntegrationField)
							{
								lowestIntegrationField = neighborTile.integrationField;
								// Calculate the vector pointing from the center of the current tile to the neighbor tile.
								floField = sf::Vector2f(static_cast<float>(newX - x), static_cast<float>(newY - y));
							}
						}
					}
				}

				// Normalize the vector.
				float length = std::sqrt(floField.x * floField.x + floField.y * floField.y);
				if (length > 0)
				{
					floField /= length;
				}

				currentTile.floField = floField;
			}
		}
	}
}




#include <vector>

void Game::FindPath()
{
	// Create a vector to store the path.
	std::vector<sf::Vector2i> path;

	sf::Vector2i currentPos = startPos;

	while (currentPos != goalPos)
	{
		// Find the neighboring tile with the smallest FloField vector.
		Tile& currentTile = tiles[currentPos.x][currentPos.y];
		sf::Vector2f minFloField = currentTile.floField;
		sf::Vector2i nextPos;

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
					if (neighborTile.floField.x < minFloField.x ||
						(neighborTile.floField.x == minFloField.x && neighborTile.floField.y < minFloField.y))
					{
						minFloField = neighborTile.floField;
						nextPos = sf::Vector2i(newX, newY);
					}
				}
			}
		}

		if (minFloField == sf::Vector2f(0.0f, 0.0f))
		{
			// No valid path found.
			break;
		}

		// Update the current position and add it to the path.
		currentPos = nextPos;
		path.push_back(currentPos);
		tiles[currentPos.x][currentPos.y].isStartTile = true;
	}

	// Update the path visualization.
	for (const sf::Vector2i& pos : path)
	{
		tiles[pos.x][pos.y].UpdateTextOnScreen();
	}
}








