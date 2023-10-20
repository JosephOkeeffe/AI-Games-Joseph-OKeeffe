#pragma once
#include <SFML/Graphics.hpp>
class Grid
{
public:
	Grid(sf::RenderWindow& window);
	void Init();
	void Render();
	void Update();
	void SetupGrid();

	bool showGrid = true;

	static const int ROWS = 50;
	static const int COLS = 50;
	int cellSize = 26;
	float gridThickness = 0.5;
	sf::RectangleShape cell[ROWS][COLS];

	sf::RenderWindow& window;
};

