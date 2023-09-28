#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include"Textures.h"
#include"Player.h"
#include "Enemy.h"
#include "SeekBehaviour.h"
#include "WandererBehaviour.h"
#include "ArriveFast.h"
#include "ArriveSlow.h"
#include "PursueBehaviour.h"



class Game
{
public:
	Game();
	~Game();
	void run();

private:

	void Init();
	void ProcessKeys(sf::Event t_event);
	void ProcessEvents();
	void Update(sf::Time t_deltaTime);
	void Render();

	const unsigned int SCREEN_WIDTH = 1800;
	const unsigned int SCREEN_HEIGHT = 800;

	sf::RenderWindow m_window;
	Textures m_textures;
	Player m_player;
	Enemy m_wanderer;
	Enemy m_seeker;
	Enemy m_arriveSlow;
	Enemy m_arriveFast;
	Enemy m_pursue;

	sf::Vector2f m_wandererPos = { 500, 700 };
	sf::Vector2f m_seekerPos = { 1700, 700 };
	sf::Vector2f m_arriveFastPos = { 1200, 700 };
	sf::Vector2f m_arriveSlowPos = { 800, 700 };
	sf::Vector2f m_pursuePos = { 1000, 7000 };
	
	sf::Font m_font;
	sf::Text m_uiText;

	bool m_exitGame;

	void DisableEnableEnemeis();

	bool showWanderer = true;
	bool showSeeker = true;
	bool showArriveFast = true;
	bool showArriveSlow = true;
	bool showPursue = true;
};

#endif

