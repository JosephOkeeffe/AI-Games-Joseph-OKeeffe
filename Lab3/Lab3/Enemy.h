#pragma once

#include <SFML/Graphics.hpp>
#include "Behaviour.h"
#include "SteeringOutput.h"

class Enemy
{
public:
    Enemy(sf::Texture& texture, std::string name);
    void Init();
    void Render(sf::RenderWindow& window);
    void Update(sf::RenderWindow& window);
    void WrapAround(sf::RenderWindow& window);
    sf::Vector2f GetPosition();
    float GetRotation();
    void CalculateVisionCone();

    inline void SetBehaviour(Behaviour* newBehaviour) 
    {
        currentBehaviour = newBehaviour;
    }

private:
    Behaviour* currentBehaviour;

    float scale = 0.5f;

    sf::Texture& m_enemyTexture;
    sf::Sprite enemySprite;

    sf::VertexArray visionCone{ sf::Triangles, 3 };
    sf::VertexArray line{ sf::Lines, 2 };

    sf::Vector2f coneVertex1;
    sf::Vector2f coneVertex2;
    sf::Vector2f coneVertex3;

    sf::Color red = sf::Color(255, 0, 0, 180);
    sf::Color black = sf::Color(0, 0, 0, 90);

    float coneLength = 200;
    float angleOffset = 200;

    std::string m_name;
    sf::Font m_font;
    sf::Text m_text;
    sf::Vector2f textOffset{ -50, 50 };
};
