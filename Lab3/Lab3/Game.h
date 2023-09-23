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
//#include "Pursue.h"



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

	sf::RenderWindow m_window;
	Textures m_textures;
	Player m_player;
	Enemy m_wanderer;
	Enemy m_seeker;
	Enemy m_arriveSlow;
	Enemy m_arriveFast;
	Enemy m_pursue;
	//std::vector<Enemy> m_enemies;

	bool m_exitGame;

};

#endif

