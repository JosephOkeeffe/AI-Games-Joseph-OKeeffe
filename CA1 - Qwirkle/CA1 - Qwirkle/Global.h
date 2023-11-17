#pragma once

struct Global
{
public:

	static const int S_WIDTH{ 1500U };
	static const int S_HEIGHT{ 1500U };
	static const int ROWS_COLUMNS{ 20 }; // rows x columns
	static const int CELL_SIZE{ S_WIDTH / ROWS_COLUMNS };

    static sf::Vector2i GetCurrentCell(sf::RenderWindow& window, sf::View& view)
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2i cellPosition;
        cellPosition.x = mousePos.x / Global::CELL_SIZE;
        cellPosition.y = mousePos.y / Global::CELL_SIZE;



        return cellPosition;
    }

	static sf::Vector2i GetMousePos(sf::RenderWindow& window)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		return mousePos;
	}
};