
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
	void update(sf::Time t_deltaTime);
	void render();
	void init();
	void initTiles();
	void ProcessMouseDown(sf::Event t_event);


	sf::RenderWindow m_window; 
	sf::Font m_ArialBlackfont;

	Tile** tiles;

	Tile** playerTiles;
	Tile** aiTiles;

	Piece selectedPiece;

	bool m_exitGame; 

};

#endif

