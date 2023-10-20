
#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "Tile.h"

class Game
{
public:
	Game();
	~Game();
	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void processMouse(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void init();
	void render();

	sf::Vector2i GetCurrentCell();

	
	void setupSprite();

	sf::RenderWindow m_window; 
	bool m_exitGame;

	//Grid grid;
	static const int ROWS = 50;
	static const int COLS = 50;
	int cellSize = 26;
	Tile tile[ROWS][COLS];

	bool isStartTile = false;
	bool isGoalTile = false;

	sf::Vector2i startPos;
	sf::Vector2i goalPos;
};

#endif

