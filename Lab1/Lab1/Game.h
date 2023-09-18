#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "EnemyWander.h"
#include "EnemySeeker.h"
#include "EnemyFlee.h"


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
	sf::VideoMode desktopMode;
	sf::Texture m_logoTexture;
	sf::Sprite m_logoSprite; 

	Player player;
	EnemyWander enemy;
	EnemySeeker seeker;
	EnemyFlee fleer;

	bool m_exitGame; 

};

#endif

