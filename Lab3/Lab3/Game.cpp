#include "Game.h"
#include <iostream>

Game::Game() :

	//desktopMode{ sf::VideoMode::getDesktopMode() },
	m_window{ sf::VideoMode{1200U, 800U, 32U}, "Lab 3" },
	m_exitGame{ false },
	m_wanderer(m_textures.m_wandererTexture, "Wanderer"),
	m_seeker(m_textures.m_seekerTexture, "Seeker"),
	m_arriveFast(m_textures.m_arriveFastTexture, "Arrive Fast"),
	m_arriveSlow(m_textures.m_arriveSlowTexture, "Arrive Slow"),
	m_pursue(m_textures.m_pursueTexture, "Pursue")
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
	std::srand(time(nullptr));
	m_player.Init();
	m_seeker.Init();
	m_wanderer.Init();
	m_arriveFast.Init();
	m_seeker.SetBehaviour(new SeekBehaviour(m_seeker, m_player));
	m_wanderer.SetBehaviour(new WandererBehaviour(m_wanderer, m_player));
	m_arriveFast.SetBehaviour(new ArriveFast(m_arriveFast, m_player));
	m_arriveSlow.SetBehaviour(new ArriveSlow(m_arriveSlow, m_player));
	m_pursue.SetBehaviour(new SeekBehaviour(m_pursue, m_player));
	//m_pursue.SetBehaviour(new PursueBehaviour(m_pursue, m_player));
	//m_enemies.push_back(m_seeker);
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
	m_seeker.Update(m_window);
	m_wanderer.Update(m_window);
	m_arriveFast.Update(m_window);
	m_arriveSlow.Update(m_window);
	m_pursue.Update(m_window);

}


void Game::Render()
{
	m_window.clear(sf::Color::White);
	m_player.Render(m_window);
	m_seeker.Render(m_window);
	m_wanderer.Render(m_window);
	m_arriveFast.Render(m_window);
	m_arriveSlow.Render(m_window);
	m_pursue.Render(m_window);

	m_window.display();
}



