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
}

void Player::Update(sf::RenderWindow& window)
{
    ChangeVelocity();
    Move();
    // Calculate the rotation angle in radians
    float angle = std::atan2(velocity.y, velocity.x);
    // Convert radians to degrees for the rotation
    float degrees = angle * (180.0f / 3.14159265359f);
    playerSprite.setPosition(currentPosition);
    WrapAround(window);
}

sf::Vector2f Player::GetPlayerPos()
{
    return playerSprite.getPosition();
}

void Player::ChangeVelocity()
{

    float directionRadians = (rotation * 3.18 / 180.0f);

    sf::Vector2f direction(std::cos(directionRadians), std::sin(directionRadians));


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        velocity += direction * acceleration;

        // Limit the speed
        if (VectorLength(velocity) > MAX_SPEED)
        {
            velocity = Normalise(velocity) * MAX_SPEED;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        velocity -= (direction ) * acceleration;
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
    currentPosition.x += velocity.x;
    currentPosition.y += velocity.y;

    playerSprite.setRotation(rotation + 90);

    playerSprite.setPosition(currentPosition);
}


void Player::WrapAround(sf::RenderWindow& window)
{
    if (currentPosition.x > window.getSize().x)
    {
        currentPosition.x = 0;
    }
    else if (currentPosition.x < 0)
    {
        currentPosition.x = window.getSize().x;
    }
    if (currentPosition.y > window.getSize().y)
    {
        currentPosition.y = 0;
    }
    else if (currentPosition.y < 0)
    {
        currentPosition.y = window.getSize().y;
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


