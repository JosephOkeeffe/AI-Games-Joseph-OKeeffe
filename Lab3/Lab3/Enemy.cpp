#include <iostream>
#include "Enemy.h"


Enemy::Enemy(sf::Texture& texture, std::string name, sf::Vector2f position/*, Player& player*/) : m_enemyTexture(texture), m_name(name), m_startingPos(position)/*, m_player(player)*/
{
}

void Enemy::Init()
{
    enemySprite.setTexture(m_enemyTexture);
    enemySprite.setPosition(m_startingPos);
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

void Enemy::Update(sf::RenderWindow& window, sf::Vector2f pos)
{
    SteeringOutput steeringOutput = currentBehaviour->GetSteering();

    m_velocity = steeringOutput.linear;
    enemySprite.move(m_velocity);
    //enemySprite.setRotation(steeringOutput.angular);
    WrapAround(window);
    CalculateVisionCone();
    m_text.setPosition(GetPosition() + textOffset);
    IsInsideVisionCone(pos);


    //float angle = std::atan2(m_velocity.y, m_velocity.x);
    //// Convert radians to degrees for the rotation
    //float degrees = angle * (180.0f / 3.14159265359f);
    //enemySprite.setRotation(degrees + steeringOutput.angular); // Set the rotation angle


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

sf::Vector2f Enemy::GetVelocity()
{
    return m_velocity;
}

float Enemy::GetOrientation()
{
    return m_orientation;
}

void Enemy::ChangeColor(sf::Color color)
{
    enemySprite.setColor(color);
}


void Enemy::CalculateVisionCone()
{
    coneVertex1 = GetPosition();
    coneVertex2 = GetPosition() + sf::Vector2f(cos((enemySprite.getRotation() + 90 + angleOffset) * PI / 180) * coneLength, 
                                                sin((enemySprite.getRotation() + 90 + angleOffset) * PI / 180) * coneLength);
    coneVertex3 = GetPosition() + sf::Vector2f(cos((enemySprite.getRotation() + 90 - angleOffset) * PI / 180) * coneLength, 
                                                sin((enemySprite.getRotation() + 90 - angleOffset) * PI / 180) * coneLength);


    visionCone[0].position = coneVertex1;
    visionCone[0].color = red;

    visionCone[1].position = coneVertex2;
    visionCone[1].color = black;

    visionCone[2].position = coneVertex3;
    visionCone[2].color = black;

    sf::Vector2f lineEndpoint = GetPosition() + sf::Vector2f(cos((enemySprite.getRotation() - 90) * 3.14159265 / 180) * coneLength, sin((enemySprite.getRotation() - 90) * 3.14159265 / 180) * coneLength);

    line[0].position = GetPosition();
    line[1].position = lineEndpoint;

    line[0].color = sf::Color::Black;
    line[1].color = sf::Color::Black;
}

bool Enemy::IsInsideVisionCone(const sf::Vector2f& point) 
{
    sf::Vector2f vector1 = point - coneVertex1;
    sf::Vector2f vector2 = point - coneVertex2;
    sf::Vector2f vector3 = point - coneVertex3;

    float angle1 = atan2(vector1.y, vector1.x) * 180.0f / 3.14159265;
    float angle2 = atan2(vector2.y, vector2.x) * 180.0f / 3.14159265;
    float angle3 = atan2(vector3.y, vector3.x) * 180.0f / 3.14159265;

    while (angle1 < 0) angle1 += 360;
    while (angle2 < 0) angle2 += 360;
    while (angle3 < 0) angle3 += 360;

    float angleA = atan2(coneVertex2.y - coneVertex1.y, coneVertex2.x - coneVertex1.x) * 180.0f / 3.14159265;
    float angleB = atan2(coneVertex3.y - coneVertex2.y, coneVertex3.x - coneVertex2.x) * 180.0f / 3.14159265;
    float angleC = atan2(coneVertex1.y - coneVertex3.y, coneVertex1.x - coneVertex3.x) * 180.0f / 3.14159265;

    while (angleA < 0) angleA += 360;
    while (angleB < 0) angleB += 360;
    while (angleC < 0) angleC += 360;

    if (angle1 >= angleA && angle1 <= angleB && angle2 >= angleB && angle2 <= angleC && angle3 >= angleC && angle3 <= angleA)
    {
        std::cout << "In \n";
        return true;
    }
    else {
        return false;
    }
}





