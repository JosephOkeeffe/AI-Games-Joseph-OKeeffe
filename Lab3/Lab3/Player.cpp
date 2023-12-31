#include <iostream>
#include "Player.h"

Player::Player()
{

}

void Player::Init()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    if (!playerTexture.loadFromFile("Images\\Player.png"))
    {
        std::cout << "problem loading Player" << std::endl;
    }

    playerSprite.setTexture(playerTexture);
    playerSprite.setOrigin(playerSprite.getTextureRect().width / 2, playerSprite.getTextureRect().height / 2);

    playerSprite.setPosition(200, 200);
    playerSprite.setScale(scale, scale);
}

void Player::Render(sf::RenderWindow& window)
{
    window.draw(playerSprite);
    window.draw(visionCone);
    window.draw(line);
}

void Player::Update(sf::RenderWindow& window)
{
    ChangeVelocity();
    Move();
    CalculateVisionCone();
    WrapAround(window);
}

sf::Vector2f Player::GetPlayerPos()
{
    sf::Vector2f pos = playerSprite.getPosition();
    return pos;
}

sf::Vector2f Player::GetVelocity()
{
    return m_velocity;
}

void Player::SetPosition(sf::Vector2f pos)
{
    playerSprite.setPosition(pos);
}

void Player::ChangeVelocity()
{
    float rotationRadians = (rotation * 3.14159265f / 180.0f);
    sf::Vector2f direction(std::cos(rotationRadians), std::sin(rotationRadians));

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        m_velocity += direction * acceleration;

        if (VectorLength(m_velocity) > MAX_SPEED)
        {
            m_velocity = Normalise(m_velocity) * MAX_SPEED;
        }
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        m_velocity -= direction * acceleration; 

        if (VectorLength(m_velocity) > MAX_SPEED)
        {
            m_velocity = Normalise(m_velocity) * MAX_SPEED;
        }
    }
    else
    {
        if (VectorLength(m_velocity) > 0.0f)
        {
            sf::Vector2f velocityDirection = Normalise(m_velocity);

            m_velocity -= velocityDirection * (acceleration / 2);

            if (VectorLength(m_velocity) < 0.2f)
            {
                m_velocity = { 0,0 };
            }
        }
    }


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        rotation += rotationSpeed;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        rotation -= rotationSpeed;
    }

}

void Player::Move()
{
    playerSprite.move(m_velocity);
    playerSprite.setRotation(rotation + 90);
}


void Player::WrapAround(sf::RenderWindow& window)
{
    if (GetPlayerPos().x > window.getSize().x)
    {
        playerSprite.setPosition(0, GetPlayerPos().y);
    }
    else if (GetPlayerPos().x < 0)
    {
        playerSprite.setPosition(window.getSize().x, GetPlayerPos().y);
    }
    if (GetPlayerPos().y > window.getSize().y)
    {
        playerSprite.setPosition(GetPlayerPos().x, 0);
    }
    else if (GetPlayerPos().y < 0)
    {
        playerSprite.setPosition(GetPlayerPos().x, window.getSize().y);
    }
}

sf::Vector2f Player::Normalise(sf::Vector2f vector)
{
    sf::Vector2f normalised = { 0.0f, 0.0f };
    float length = VectorLength(vector);

    if (length == 0.0f)
        return vector;

    normalised.x = vector.x / length;
    normalised.y = vector.y / length;

    return normalised;
}

float Player::VectorLength(sf::Vector2f vector)
{
    float vLength = std::sqrt(vector.x * vector.x + vector.y * vector.y);
    return vLength;
}

void Player::CalculateVisionCone()
{
    coneVertex1 = GetPlayerPos();
    coneVertex2 = GetPlayerPos() + sf::Vector2f(cos((playerSprite.getRotation() + 90 + angleOffset) * 3.14159265 / 180) * coneLength, sin((playerSprite.getRotation() + 90 + angleOffset) * 3.14159265 / 180) * coneLength);
    coneVertex3 = GetPlayerPos() + sf::Vector2f(cos((playerSprite.getRotation() + 90 - angleOffset) * 3.14159265 / 180) * coneLength, sin((playerSprite.getRotation() + 90 - angleOffset) * 3.14159265 / 180) * coneLength);


    visionCone[0].position = coneVertex1;
    visionCone[0].color = startColor; 

    visionCone[1].position = coneVertex2;
    visionCone[1].color = endColor;

    visionCone[2].position = coneVertex3;
    visionCone[2].color = endColor;
    //

    sf::Vector2f lineEndpoint = GetPlayerPos() + sf::Vector2f(cos((playerSprite.getRotation() - 90  ) * 3.14159265 / 180) * coneLength, sin((playerSprite.getRotation() - 90 ) * 3.14159265 / 180) * coneLength);

    line[0].position = GetPlayerPos();
    line[1].position = lineEndpoint;

    line[0].color = sf::Color::Black;
    line[1].color = sf::Color::Black;
}


