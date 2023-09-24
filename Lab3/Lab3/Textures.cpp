#include "Textures.h"
#include <iostream>

Textures::Textures()
{
    if (!m_playerTexture.loadFromFile("Images\\Player.png")) { std::cout << "problem loading Player" << std::endl; }

    if (!m_wandererTexture.loadFromFile("Images\\Jacko.png")) { std::cout << "problem loading Wanderer" << std::endl; }
    if (!m_seekerTexture.loadFromFile("Images\\Spitzy.png")) { std::cout << "problem loading Seeker" << std::endl; }
    if (!m_arriveFastTexture.loadFromFile("Images\\Mosu.png")) { std::cout << "problem loading Arrive Fast" << std::endl; }
    if (!m_arriveSlowTexture.loadFromFile("Images\\Grollont.png")) { std::cout << "problem loading Arrive Slow" << std::endl; }
    if (!m_pursueTexture.loadFromFile("Images\\Gorkk.png")) { std::cout << "problem loading Pursue" << std::endl; }
}
