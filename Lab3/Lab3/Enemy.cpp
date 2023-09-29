#include <iostream>
#include "Enemy.h"


Enemy::Enemy(sf::Texture& texture, std::string name, sf::Vector2f position) : m_enemyTexture(texture), m_name(name), m_startingPos(position)
{
}

void Enemy::Init()
{
    m_enemySprite.setTexture(m_enemyTexture);
    m_enemySprite.setPosition(m_startingPos);
    m_enemySprite.setOrigin(m_enemySprite.getTextureRect().width / 2, m_enemySprite.getTextureRect().height / 2);
    m_enemySprite.setScale(scale, scale);
    
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
    window.draw(m_enemySprite);
    window.draw(m_text);
}

void Enemy::Update(sf::RenderWindow& window, sf::Vector2f pos)
{
    SteeringOutput steeringOutput = currentBehaviour->GetSteering();

    m_velocity = steeringOutput.linear;
    m_enemySprite.move(m_velocity);
    m_enemySprite.setRotation(steeringOutput.angular + 90);
    WrapAround(window);
    CalculateVisionCone();
    m_text.setPosition(GetPosition() + textOffset);
    IsInsideVisionCone(pos);


}

void Enemy::WrapAround(sf::RenderWindow& window)
{
    if (GetPosition().x > window.getSize().x)
    {
        m_enemySprite.setPosition(0, GetPosition().y);
    }
    else if (GetPosition().x < 0)
    {
        m_enemySprite.setPosition(window.getSize().x, GetPosition().y);
    }
    if (GetPosition().y > window.getSize().y)
    {
        m_enemySprite.setPosition(GetPosition().x, 0);
    }
    else if (GetPosition().y < 0)
    {
        m_enemySprite.setPosition(GetPosition().x, window.getSize().y);
    }
}

sf::Vector2f Enemy::GetPosition()
{
    return m_enemySprite.getPosition();
}

float Enemy::GetRotation()
{
    return m_enemySprite.getRotation();
}

sf::Vector2f Enemy::GetVelocity()
{
    return m_velocity;
}

float Enemy::GetOrientation()
{
    return m_orientation;
}

void Enemy::SetVelocity(sf::Vector2f velocity)
{
    m_velocity = velocity;
}

void Enemy::ChangeColor(sf::Color color)
{
    m_enemySprite.setColor(color);
}


void Enemy::CalculateVisionCone()
{
    coneVertex1 = GetPosition();
    coneVertex2 = GetPosition() + sf::Vector2f(cos((m_enemySprite.getRotation() + 90 + angleOffset) * PI / 180) * coneLength, 
                                                sin((m_enemySprite.getRotation() + 90 + angleOffset) * PI / 180) * coneLength);
    coneVertex3 = GetPosition() + sf::Vector2f(cos((m_enemySprite.getRotation() + 90 - angleOffset) * PI / 180) * coneLength, 
                                                sin((m_enemySprite.getRotation() + 90 - angleOffset) * PI / 180) * coneLength);

    visionCone[0].position = coneVertex1;
    visionCone[0].color = currentColor;

    visionCone[1].position = coneVertex2;
    visionCone[1].color = m_black;

    visionCone[2].position = coneVertex3;
    visionCone[2].color = m_black;

    sf::Vector2f lineEndpoint = GetPosition() + sf::Vector2f(cos((m_enemySprite.getRotation() - 90) * PI / 180) * coneLength, 
                                                              sin((m_enemySprite.getRotation() - 90) * PI / 180) * coneLength);

    line[0].position = GetPosition();
    line[1].position = lineEndpoint;

    line[0].color = m_black;
    line[1].color = m_black;
}

bool Enemy::IsInsideVisionCone(const sf::Vector2f& playerPos) 
{
    // distance from player to enemy
    sf::Vector2f distance = playerPos - GetPosition();

    float distanceSquared = std::sqrt(distance.x * distance.x + distance.y * distance.y);

    float currentRotation = m_enemySprite.getRotation();

    float angle = std::atan2(distance.y, distance.x);

    angle = angle * 180.0f / PI;

    angle -= currentRotation;

    float halfVisionCone = angleOffset / 2;

    if (angle < -180.0f)
    {
        angle += 360.0f;
    }
    else if (angle > 180.0f)
    {
        angle -= 360.0f;
    }

    if (distanceSquared <= (coneLength + 40) 
        && angle >= -halfVisionCone 
        && angle <= halfVisionCone) 
    {
        currentColor = m_red;
        return true;
    }
    else 
     {
        currentColor = m_green;
        return false;

    }
}





