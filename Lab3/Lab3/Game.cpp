#include "Game.h"
#include <iostream>

Game::Game() :

	//desktopMode{ sf::VideoMode::getDesktopMode() },
	m_window{ sf::VideoMode{SCREEN_WIDTH,SCREEN_HEIGHT, 32U}, "Lab 3" },
	m_exitGame{ false },
	m_wanderer(m_textures.m_wandererTexture, "Wanderer", m_wandererPos),
	m_seeker(m_textures.m_seekerTexture, "Seeker", m_seekerPos),
	m_arriveFast(m_textures.m_arriveFastTexture, "Arrive Fast", m_arriveFastPos),
	m_arriveSlow(m_textures.m_arriveSlowTexture, "Arrive Slow", m_arriveSlowPos),
	m_pursue(m_textures.m_pursueTexture, "Pursue", m_pursuePos)
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
	m_arriveSlow.Init();
	m_pursue.Init();
	m_seeker.SetBehaviour(new SeekBehaviour(m_seeker, m_player));
	m_wanderer.SetBehaviour(new WandererBehaviour(m_wanderer, m_player));
	m_arriveFast.SetBehaviour(new ArriveFast(m_arriveFast, m_player));
	m_arriveSlow.SetBehaviour(new ArriveSlow(m_arriveSlow, m_player));
	m_pursue.SetBehaviour(new PursueBehaviour(m_pursue, m_player));

	if (!m_font.loadFromFile("Fonts\\ariblk.ttf")) { std::cout << "problem loading font" << std::endl; }
	m_uiText.setFont(m_font);
	m_uiText.setPosition(40, 10);
	m_uiText.setCharacterSize(32);
	m_uiText.setFillColor(sf::Color::Black);
	m_uiText.setOutlineColor(sf::Color::Red);
	m_uiText.setOutlineThickness(1);
	m_uiText.setString("1 - Wanderer, 2 - Seeker, 3 - Arrive Fast, 4 Arrive Slow, 5 - Pursue");

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

	DisableEnableEnemeis();
	
}

void Game::Update(sf::Time t_deltaTime)
{
	if (m_exitGame) { m_window.close(); }


	m_player.Update(m_window); 
	m_seeker.Update(m_window, m_player.GetPlayerPos());
	m_wanderer.Update(m_window, m_player.GetPlayerPos());
	m_arriveFast.Update(m_window, m_player.GetPlayerPos());
	m_arriveSlow.Update(m_window, m_player.GetPlayerPos());
	m_pursue.Update(m_window, m_player.GetPlayerPos());

}


void Game::Render()
{
	m_window.clear(sf::Color::White);
	m_player.Render(m_window);
	if (showWanderer) { m_wanderer.Render(m_window);}
	if (showSeeker) { m_seeker.Render(m_window);}
	if (showArriveFast) { m_arriveFast.Render(m_window); }
	if (showArriveSlow) { m_arriveSlow.Render(m_window); }
	if (showPursue) { m_pursue.Render(m_window);}

	m_window.draw(m_uiText);
	
	m_window.display();
}

void Game::DisableEnableEnemeis()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		showWanderer = !showWanderer;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		showSeeker = !showSeeker;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
	{
		showArriveFast = !showArriveFast;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
	{
		showArriveSlow = !showArriveSlow;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
	{
		showPursue = !showPursue;
	}
}



