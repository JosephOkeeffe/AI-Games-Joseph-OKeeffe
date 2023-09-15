#include "Game.h"
#include <iostream>

Game::Game() :
	m_window{ sf::VideoMode{ 1600U, 800U, 32U }, "SFML Game" },
	m_exitGame{ false } //when true game will exit
{
	Init();
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
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			Update(timePerFrame); //60 fps
		}
		Render(); // as many as possible
	}
}

void Game::Init()
{
	player.Init();
	enemy.Init();
}

void Game::ProcessKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
}

void Game::Update(sf::Time t_deltaTime)
{
	if (m_exitGame){ m_window.close(); }

	player.Update(m_window);
	enemy.Update(m_window);
}


void Game::Render()
{
	m_window.clear(sf::Color::White);
	player.Render(m_window);
	enemy.Render(m_window);

	m_window.display();
}



