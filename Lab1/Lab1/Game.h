/// <summary>
/// author Pete Lowe May 2019
/// you need to change the above line or lose marks
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"


class Game
{
public:
	Game();
	~Game();
	void run();

private:

	void Init();
	void ProcessKeys(sf::Event t_event);
	void Update(sf::Time t_deltaTime);
	void Render();


	sf::RenderWindow m_window; 
	sf::Texture m_logoTexture;
	sf::Sprite m_logoSprite; 

	Player player;
	Enemy enemy;

	bool m_exitGame; 

};

#endif

