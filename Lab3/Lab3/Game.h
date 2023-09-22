#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include"Player.h"
#include "Enemy.h"
#include "SeekBehaviour.h"



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

	Player m_player;
	Enemy m_enemy;
	//SeekBehaviour m_seekBehaviour;

	bool m_exitGame;

};

#endif

