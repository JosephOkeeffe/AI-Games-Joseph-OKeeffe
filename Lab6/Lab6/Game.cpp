#include "Game.h"
#include <iostream>
#include "Global.h"
#include <queue>

Game::Game() :
	m_window{ sf::VideoMode{ Global::S_WIDTH, Global::S_HEIGHT, 32U }, "SFML Game" },
	m_exitGame{false}
{
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
}
void Game::processMouse(sf::Event t_event)
{
	SetStart(t_event);
	SetObstacle(t_event);
	SetGoal(t_event);
}

void Game::SetStart(sf::Event t_event)
{
	if (sf::Mouse::Left == t_event.key.code)
	{
		currentPathIndex = 0;

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
		circle.setPosition(tiles[startPos.x][startPos.y].tile.getPosition().x + (circle.getRadius()), tiles[startPos.x][startPos.y].tile.getPosition().y + (circle.getRadius()));

		if (isGoalTile)
		{
			FindPath();
		}
	}
}

void Game::SetObstacle(sf::Event t_event)
{
	if (sf::Mouse::Middle == t_event.key.code)
	{

		tiles[GetCurrentCell().x][GetCurrentCell().y].SetObstacle();
		tiles[GetCurrentCell().x][GetCurrentCell().y].cost = 999;
		tiles[GetCurrentCell().x][GetCurrentCell().y].integrationField = 999;
		if (isGoalTile && isStartTile)
		{
			BushFire();
			CalculateFloField();
			FindPath();
		}
	}
}

void Game::SetGoal(sf::Event t_event)
{
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
			UpdateColor();
			FindPath();
		}
	}
}

void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame){m_window.close();}
	if (!isStartTile || !isGoalTile) { return; }

	MoveCircle();

}

void Game::init()
{
	font.loadFromFile("./ASSETS/FONTS/ariblk.ttf");

	elapsed = timer.getElapsedTime();
	UpdateText();

	circle.setRadius(10);
	circle.setFillColor(sf::Color::Magenta);
	circle.setOutlineThickness(1);
	circle.setOutlineColor(sf::Color::Black);

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
	m_window.clear(sf::Color(0,0,255, 100));

	for (int row = 0; row < Global::ROWS_COLUMNS; row++)
	{
		for (int col = 0; col < Global::ROWS_COLUMNS; col++)
		{
			tiles[row][col].line[0] = tiles[row][col].tile.getPosition() + sf::Vector2f(Global::CELL_SIZE / 2, Global::CELL_SIZE / 2);
			tiles[row][col].line[1] = tiles[row][col].tile.getPosition() + sf::Vector2f(Global::CELL_SIZE / 2, Global::CELL_SIZE / 2) + tiles[row][col].floField * static_cast<float>(Global::CELL_SIZE * 1.5);
			tiles[row][col].line[0].color = sf::Color::Black;
			tiles[row][col].line[1].color = sf::Color::Red;
			tiles[row][col].Render(m_window);
		}
	}

	m_window.draw(circle);
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

}

void Game::MoveCircle()
{
	elapsed = timer.getElapsedTime();

	if (elapsed.asSeconds() >= moveInterval)
	{
		timer.restart(); 

		if (currentPathIndex < path.size()) 
		{
			sf::Vector2i nextPosition = path[currentPathIndex];
			tiles[nextPosition.x][nextPosition.y].tile.getPosition();
			
			circle.setPosition(tiles[nextPosition.x][nextPosition.y].tile.getPosition().x + (circle.getRadius()), tiles[nextPosition.x][nextPosition.y].tile.getPosition().y + (circle.getRadius()));
			currentPathIndex++;
		}
	}
}

sf::Color Game::GradientColour(int x)
{
	sf::Uint8 grad = static_cast<sf::Uint8>((255 * x) / 60);
	sf::Color gradientColor = sf::Color(grad, grad, grad); // Creating a color with varying red component

	return gradientColor;
}

void Game::UpdateColor()
{
	for (int x = 0; x < Global::ROWS_COLUMNS; x++)
	{
		for (int y = 0; y < Global::ROWS_COLUMNS; y++)
		{
			Tile& currentTile = tiles[x][y];
			currentTile.currentColour = GradientColour(currentTile.cost);
			currentTile.tile.setFillColor(GradientColour(currentTile.cost));
			currentTile.CheckStatus();
		}
	}
}

sf::Vector2i Game::GetCurrentCell()
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
	mousePos /= Global::CELL_SIZE;
	return mousePos;
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
					Tile& neighbourTile = tiles[newX][newY];
					neighbourTile.UpdateTextOnScreen();

					int newCost = currentTile.cost + 1; 

					if (newCost < neighbourTile.cost)
					{
						neighbourTile.cost = newCost;

						if (!neighbourTile.isObstacleTile)
						{
							neighbourTile.UpdateTextOnScreen();

							sf::Vector2f goalPosition = tiles[goalPos.x][goalPos.y].tile.getPosition();
							sf::Vector2f currentPosition = tiles[currentPos.x][currentPos.y].tile.getPosition();

							int dx = std::abs(currentPosition.x - goalPosition.x);
							int dy = std::abs(currentPosition.y - goalPosition.y);

		
							int directDistance = dx + dy;

							neighbourTile.integrationField = directDistance + neighbourTile.cost;

							queue.push(sf::Vector2i(newX, newY));
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
			sf::Vector2f floField(0.0f, 0.0f);

			if (!currentTile.isObstacleTile)
			{
				int lowestIntegrationField = currentTile.integrationField;

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
								floField = sf::Vector2f(static_cast<float>(newX - x), static_cast<float>(newY - y));
							}
						}
					}
				}
				currentTile.floField = floField;
			}
		}
	}
}

void Game::FindPath()
{
	for (sf::Vector2i x : path)
	{
		tiles[x.x][x.y].tile.setFillColor(tiles[x.x][x.y].currentColour);
		UpdateColor();
	}

	sf::Vector2i currentPos = startPos;
	path.clear();

	if (isStartTile)
	{


		while (currentPos != goalPos)
		{
			Tile& currentTile = tiles[currentPos.x][currentPos.y];
			if (!currentTile.isObstacleTile)
			{
				sf::Vector2f temp = currentTile.floField;
				currentPos.x += temp.x;
				currentPos.y += temp.y;

				tiles[currentPos.x][currentPos.y].tile.setFillColor(sf::Color::Yellow);
				path.push_back(currentPos);
			}
		}
		tiles[goalPos.x][goalPos.y].tile.setFillColor(sf::Color::Red);
	}
}








