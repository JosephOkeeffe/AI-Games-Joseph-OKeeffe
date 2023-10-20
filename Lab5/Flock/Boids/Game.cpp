#include "Game.h"
#include <iostream>
using namespace std;

Game::Game()
{
	window_height = desktop.height;
	window_width = desktop.width;

	//Having the style of "None" gives a false-fullscreen effect for easier closing and access.
	m_window.create(sf::VideoMode(window_width - 100, window_height - 100, desktop.bitsPerPixel), "Flocking", sf::Style::None);
	m_window.setVerticalSyncEnabled(true);
	m_exitGame = false; //when true game will exit

	m_actionMessage.setFont(m_font);
	m_actionMessage.setString("Flocking");
	m_actionMessage.setFillColor(sf::Color::White);

	if (!m_font.loadFromFile("assets/fonts/ariblk.ttf"))
	{
		std::cout << "failed loading ariblk.ttf font file" << std::endl;
	}
	else
		std::cout << "successfully loaded ariblk.ttf font file" << std::endl;

	SetupGrid();

	for (int i = 0; i < MAX_BOIDS; i++) //Number of boids is hardcoded for testing pusposes.
	{
		//Boid b(rand() % window_width, rand() % window_height); //Starts the boid with a random position in the window.
		Boid b(window_width / 3, window_height / 3); //Starts all boids in the center of the screen
		if (i == leader)
		{
			b.isLeader = true;
		}
		sf::CircleShape shape(8, 3); //Shape with a radius of 10 and 3 points (Making it a triangle)

		//Changing the Visual Properties of the shape
		//shape.setPosition(b.location.x, b.location.y); //Sets position of shape to random location that boid was set to.
		shape.setPosition(window_width, window_height); //Testing purposes, starts all shapes in the center of screen.
		shape.setOutlineColor(sf::Color(0, 255, 0));
		shape.setFillColor(sf::Color::Green);
		shape.setOutlineColor(sf::Color::White);
		shape.setOutlineThickness(1);
		shape.setRadius(boidsSize);

		//Adding the boid to the flock and adding the shapes to the vector<sf::CircleShape>
		flock.addBoid(b);
		shapes.push_back(shape);
	}

}

Game::~Game()
{
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if (sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
		if (sf::Event::MouseButtonPressed == newEvent.type || sf::Event::MouseButtonReleased == newEvent.type) //user pressed a mouse button
		{
			processMouse(newEvent);
		}
		
	}
}

void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
	if (sf::Keyboard::Up == t_event.key.code) { flock.getBoid(leader)->accelerate(1); }
	if (sf::Keyboard::Down == t_event.key.code) { flock.getBoid(leader)->accelerate(-1); }
	if (sf::Keyboard::Left == t_event.key.code) { flock.getBoid(leader)->steer(-1); }
	if (sf::Keyboard::Right == t_event.key.code) { flock.getBoid(leader)->steer(1); }
	else if (sf::Keyboard::Space == t_event.key.code)
	{
		if (action == "flock")
		{
			action = "swarm";
		}
		else
		{
			action = "flock";
		}
	}
	else if (sf::Keyboard::Num1 == t_event.key.code)
	{
		action = "custom";
	}
	else if (sf::Keyboard::Num2 == t_event.key.code)
	{
		action = "line";
	}
	else if (sf::Keyboard::G == t_event.key.code)
	{
		showGrid = !showGrid;
	}

	

}

void Game::processMouse(sf::Event t_event)
{
	if (sf::Event::MouseButtonReleased == t_event.type && sf::Mouse::Left == t_event.mouseButton.button)
	{
		//Gets mouse coordinates, sets that as the location of the boid and the shape
		sf::Vector2i mouseCoords = sf::Mouse::getPosition(m_window);
		Boid b(mouseCoords.x, mouseCoords.y, true);
		sf::CircleShape shape(10, 3);

		//Changing visual properties of newly created boid
		shape.setPosition(mouseCoords.x, mouseCoords.y);
		shape.setOutlineColor(sf::Color(255, 0, 0));
		shape.setFillColor(sf::Color(255, 0, 0));
		shape.setOutlineColor(sf::Color::White);
		shape.setOutlineThickness(1);
		shape.setRadius(enemySize);

		//Adds newly created boid and shape to their respective data structure
		flock.addBoid(b);
		shapes.push_back(shape);
		// New Shape is drawn on next render loop
	}

}

void Game::update(sf::Time t_deltaTime)
{
	m_window.clear();


	for (int i = 0; i < shapes.size(); i++)
	{
		shapes[i].setPosition(flock.getBoid(i)->location.x, flock.getBoid(i)->location.y);

		float theta;
		theta = flock.getBoid(i)->angle(flock.getBoid(i)->velocity);
		shapes[i].setRotation(theta);

	}

	if (action == "flock")
	{
		flock.flocking();
	}
	else if (action == "custom")
	{
		flock.CustomFormation(leader);
		shapes[leader].setFillColor(sf::Color::Magenta);
		shapes[leader].setRadius(5);
	}
	else if (action == "line")
	{
		flock.LineFormation(leader);
		shapes[leader].setFillColor(sf::Color::Magenta);
		shapes[leader].setRadius(5);
	}
	else
	{

		flock.swarming();
	}

	if (m_exitGame)
	{
		m_window.close();
	}
	if (m_actionMessage.getString() != action)
		m_actionMessage.setString(action);

}

void Game::render()
{
	for (int i = 0; i < shapes.size(); i++)
	{
		m_window.draw(shapes[i], flock.transform);
		
	}

	if (showGrid)
	{
		for (int row = 0; row < ROWS; row++)
		{
			for (int col = 0; col < COLS; col++)
			{
				m_window.draw(cell[row][col]);
			}
		}
	}

	m_window.draw(m_actionMessage);
	m_window.display();
}

void Game::SetupGrid()
{
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			cell[row][col].setSize(sf::Vector2f(cellSize, cellSize));
			cell[row][col].setPosition(row * cellSize, col * cellSize);
			cell[row][col].setFillColor(sf::Color::Transparent);
			cell[row][col].setOutlineColor(sf::Color(255,255,255, 50));
			cell[row][col].setOutlineThickness(2);
		}
	}
}
