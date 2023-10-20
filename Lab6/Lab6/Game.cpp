#include "Game.h"
#include <iostream>
#include "Global.h"

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
		
		startPos = GetCurrentCell();

		if (isStartTile)
		{
			sf::Vector2i oldStartPos = startPos;
			tile[oldStartPos.x][oldStartPos.y].SetStart();
		}
		else
		{
			// reset color back
			isStartTile = true;
			tile[GetCurrentCell().x][GetCurrentCell().y].SetStart();
		}
		
		
		
		
		std::cout << "Start X:" << startPos.x << "\n";
		std::cout << "Start Y:" << startPos.y << "\n";

	}
	if (sf::Mouse::Middle == t_event.key.code)
	{
		tile[GetCurrentCell().x][GetCurrentCell().y].SetObstacle();
	}
	if (sf::Mouse::Right == t_event.key.code)
	{
		isGoalTile = true;
		tile[GetCurrentCell().x][GetCurrentCell().y].SetGoal();
		goalPos = GetCurrentCell();
		std::cout << "Goal X:" << goalPos.x << "\n";
		std::cout << "Goal Y:" << goalPos.y << "\n";
	}

}

void Game::update(sf::Time t_deltaTime)
{
	//if (m_exitGame){m_window.close();}
	//for (int row = 0; row < ROWS; row++)
	//{
	//	for (int col = 0; col < COLS; col++)
	//	{
	//		tile[row][col].GetTilePosition(m_window);
	//	}
	//}
}

void Game::init()
{
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			tile[row][col].Init(sf::Vector2f(row * cellSize, col * cellSize));
		}
	}
	
}

void Game::render()
{
	m_window.clear(sf::Color::Black);

	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			tile[row][col].Render(m_window);
		}
	}
	m_window.display();

}

sf::Vector2i Game::GetCurrentCell()
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
	mousePos /= cellSize;

	return mousePos;
}

void Game::setupSprite()
{
}

