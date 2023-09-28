#pragma once

#include <SFML/Graphics.hpp>
#include "Behaviour.h"
#include "SteeringOutput.h"
#include "Player.h"

class Enemy
{
public:
    Enemy(sf::Texture& texture, std::string name, sf::Vector2f startPos/*, Player& player*/);
    void Init();
    void Render(sf::RenderWindow& window);
    void Update(sf::RenderWindow& window, sf::Vector2f pos);
    void WrapAround(sf::RenderWindow& window);
    sf::Vector2f GetPosition();
    float GetRotation();
    sf::Vector2f GetVelocity();
    float GetOrientation();
    void ChangeColor(sf::Color color);

    bool IsInsideVisionCone(const sf::Vector2f& positionToCheck);

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
    sf::Vector2f m_startingPos;

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

    // Kinematic
    float m_orientation;
    sf::Vector2f m_velocity;

    float PI = 3.14159265;

    //Player& m_player;
    

};
