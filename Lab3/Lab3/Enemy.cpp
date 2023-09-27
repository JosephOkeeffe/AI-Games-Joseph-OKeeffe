#include <iostream>
#include "Enemy.h"


Enemy::Enemy(sf::Texture& texture, std::string name) : m_enemyTexture(texture), m_name(name)
{
}

void Enemy::Init()
{
    enemySprite.setTexture(m_enemyTexture);
    enemySprite.setPosition(200, 200);
    enemySprite.setOrigin(enemySprite.getTextureRect().width / 2, enemySprite.getTextureRect().height / 2);
    enemySprite.setScale(scale, scale);
    
    if (!m_font.loadFromFile("Fonts\\ariblk.ttf")) { std::cout << "problem loading font" << std::endl; }

    m_text.setFont(m_font);
    m_text.setCharacterSize(32);
    m_text.setFillColor(sf::Color::Black);
    m_text.setString(m_name);


}

void Enemy::Render(sf::RenderWindow& window)
{
    window.draw(visionCone);
    window.draw(line);
    window.draw(enemySprite);
    window.draw(m_text);
}

void Enemy::Update(sf::RenderWindow& window)
{
    SteeringOutput steeringOutput = currentBehaviour->GetSteering();

    enemySprite.move(steeringOutput.linear);
    enemySprite.setRotation(steeringOutput.angular + 90);
    WrapAround(window);
    CalculateVisionCone();
    m_text.setPosition(GetPosition() + textOffset);


}

void Enemy::WrapAround(sf::RenderWindow& window)
{
    if (GetPosition().x > window.getSize().x)
    {
        enemySprite.setPosition(0, GetPosition().y);
    }
    else if (GetPosition().x < 0)
    {
        enemySprite.setPosition(window.getSize().x, GetPosition().y);
    }
    if (GetPosition().y > window.getSize().y)
    {
        enemySprite.setPosition(GetPosition().x, 0);
    }
    else if (GetPosition().y < 0)
    {
        enemySprite.setPosition(GetPosition().x, window.getSize().y);
    }
}

sf::Vector2f Enemy::GetPosition()
{
    return enemySprite.getPosition();
}

float Enemy::GetRotation()
{
    return enemySprite.getRotation();
}


void Enemy::CalculateVisionCone()
{
    coneVertex1 = GetPosition();
    coneVertex2 = GetPosition() + sf::Vector2f(cos((enemySprite.getRotation() + 90 + angleOffset) * 3.14159265 / 180) * coneLength, sin((enemySprite.getRotation() + 90 + angleOffset) * 3.14159265 / 180) * coneLength);
    coneVertex3 = GetPosition() + sf::Vector2f(cos((enemySprite.getRotation() + 90 - angleOffset) * 3.14159265 / 180) * coneLength, sin((enemySprite.getRotation() + 90 - angleOffset) * 3.14159265 / 180) * coneLength);


    visionCone[0].position = coneVertex1;
    visionCone[0].color = red;

    visionCone[1].position = coneVertex2;
    visionCone[1].color = black;

    visionCone[2].position = coneVertex3;
    visionCone[2].color = black;
    //

    sf::Vector2f lineEndpoint = GetPosition() + sf::Vector2f(cos((enemySprite.getRotation() - 90) * 3.14159265 / 180) * coneLength, sin((enemySprite.getRotation() - 90) * 3.14159265 / 180) * coneLength);

    line[0].position = GetPosition();
    line[1].position = lineEndpoint;

    line[0].color = sf::Color::Black;
    line[1].color = sf::Color::Black;
}


