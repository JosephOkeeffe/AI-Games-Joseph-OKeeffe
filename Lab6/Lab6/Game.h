
#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Global.h"

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
	void initTiles();

	sf::Vector2i GetCurrentCell();
	void setupSprite();
	void BushFire();
	std::vector<sf::Vector2i> FindPath();

	sf::RenderWindow m_window; 
	bool m_exitGame;

	//Tile tiles[Global::ROWS_COLUMNS][Global::ROWS_COLUMNS];
	Tile** tiles;

	sf::Font font;


	bool isStartTile = false;
	bool isGoalTile = false;

	sf::Vector2i previousCellPos;
	sf::Vector2i currentCellPos;
	sf::Vector2i startPos;
	sf::Vector2i goalPos{ 1, 1 };
};

#endif

