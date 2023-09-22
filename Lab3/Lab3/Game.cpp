#include "Game.h"
#include <iostream>

Game::Game() :

	//desktopMode{ sf::VideoMode::getDesktopMode() },
	m_window{ sf::VideoMode{1200U, 800U, 32U}, "Lab 1" },
	m_exitGame{ false }
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
			ProcessEvents();
			Update(timePerFrame); //60 fps
		}
		Render(); // as many as possible
	}
}

void Game::Init()
{
	m_player.Init();
	m_enemy.Init();
	m_enemy.SetBehaviour(new SeekBehaviour(m_enemy, m_player));
}

void Game::ProcessEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if (sf::Event::Closed == newEvent.type)
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type)
		{
			ProcessKeys(newEvent);
		}
	}
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
	if (m_exitGame) { m_window.close(); }


	m_player.Update(m_window); 
	m_enemy.Update();

}


void Game::Render()
{
	m_window.clear(sf::Color::White);
	m_player.Render(m_window);
	m_enemy.Render(m_window);


	m_window.display();
}



