#pragma once
#include <SFML/Graphics.hpp> 

class Textures
{
public:
	Textures();

	sf::Texture m_playerTexture;
	sf::Texture m_wandererTexture;
	sf::Texture m_seekerTexture;
	sf::Texture m_arriveFastTexture;
	sf::Texture m_arriveSlowTexture;
	sf::Texture m_pursueTexture;
};

