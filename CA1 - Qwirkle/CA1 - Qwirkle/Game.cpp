
#include "Game.h"
#include <iostream>
#include <random>
#include <iostream>
#include <vector>
#include <set>

Game::Game() :
	m_window{ sf::VideoMode{ Global::S_WIDTH, Global::S_HEIGHT + 100, 32U }, "Qwirkle" },
	m_exitGame{false}
{
	srand(time(nullptr));
	init();
}


Game::~Game()
{
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps);
	while (m_window.isOpen())
	{
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			update(timePerFrame);
		}
		render(); 
	}
}
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if ( sf::Event::Closed == newEvent.type)
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) 
		{
			processKeys(newEvent);
		}
		if (isSinglePlayer)
		{
			if (sf::Event::MouseButtonPressed == newEvent.type && isPlayerTurn || state == MENU)
			{
				ProcessMouseDown(newEvent);
			}
		}
		else
		{
			if (sf::Event::MouseButtonPressed == newEvent.type || state == MENU)
			{
				ProcessMouseDown(newEvent);
			}
		}
		
	}
}

void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
}

/// <summary>
/// Handles all the mouse press events
/// </summary>
/// <param name="t_event"></param>
void Game::ProcessMouseDown(sf::Event t_event)
{
	if (sf::Mouse::Left == t_event.key.code)
	{
		if (state == MENU)
		{
			sf::Vector2f mousePos = static_cast<sf::Vector2f>(Global::GetMousePos(m_window));

			if (easyButton.getGlobalBounds().contains(mousePos))
			{
				state = GAME;
				difficulty = EASY;
				StartGame();
			}
			if (mediumButton.getGlobalBounds().contains(mousePos))
			{
				state = GAME;
				difficulty = MEDIUM;
				StartGame();
			}
			if (hardButton.getGlobalBounds().contains(mousePos))
			{
				state = GAME;
				difficulty = HARD;
				StartGame();
			}

			if (pveButton.getGlobalBounds().contains(mousePos))
			{
				pvpButton.setFillColor(sf::Color::Red);
				pveButton.setFillColor(sf::Color::Green);
				isSinglePlayer = true;
			}

			if (pvpButton.getGlobalBounds().contains(mousePos))
			{
				pvpButton.setFillColor(sf::Color::Green);
				pveButton.setFillColor(sf::Color::Red);
				isSinglePlayer = false;
			}

		}
		// GAME
		else
		{
			sf::Vector2f mousePos = static_cast<sf::Vector2f>(Global::GetMousePos(m_window));

			// If playing against another person
			if (!isSinglePlayer)
			{
				if (isPlayerTurn)
				{
					SelectPlayerTile(playerTiles);
					PlaceTileOnBoard(playerTiles);

					if (playerShuffleButton.getGlobalBounds().contains(mousePos))
					{
						if (difficulty == EASY)
						{
							if (shuffleCounter < MAX_SHUFFLE_EASY)
							{
								ShufflePlayerTiles(playerTiles);
							}
							else
							{
								playerShuffleButton.setColor(sf::Color::Red);
							}
						}
						else if (difficulty == MEDIUM)
						{
							if (shuffleCounter < MAX_SHUFFLE_MEDIUM)
							{
								ShufflePlayerTiles(playerTiles);
							}
							else
							{
								playerShuffleButton.setColor(sf::Color::Red);
							}
						}
						else if (difficulty == HARD)
						{
							if (shuffleCounter < MAX_SHUFFLE_HARD)
							{
								ShufflePlayerTiles(playerTiles);
							}
							else
							{
								playerShuffleButton.setColor(sf::Color::Red);
							}
						}
					}
				}
				else
				{
					SelectPlayerTile(aiTiles);
					PlaceTileOnBoard(aiTiles);

					if (playerShuffleButton.getGlobalBounds().contains(mousePos))
					{
						if (difficulty == EASY)
						{
							if (shuffleCounter < MAX_SHUFFLE_EASY)
							{
								ShufflePlayerTiles(aiTiles);
							}
							else
							{
								playerShuffleButton.setColor(sf::Color::Red);
							}
						}
						else if (difficulty == MEDIUM)
						{
							if (shuffleCounter < MAX_SHUFFLE_MEDIUM)
							{
								ShufflePlayerTiles(aiTiles);
							}
							else
							{
								playerShuffleButton.setColor(sf::Color::Red);
							}
						}
						else if (difficulty == HARD)
						{
							if (shuffleCounter < MAX_SHUFFLE_HARD)
							{
								ShufflePlayerTiles(aiTiles);
							}
							else
							{
								playerShuffleButton.setColor(sf::Color::Red);
							}
						}
					}

				}

				
			}
			else
			{
				if (isPlayerTurn)
				{
					SelectPlayerTile(playerTiles);
					PlaceTileOnBoard(playerTiles);

					if (playerShuffleButton.getGlobalBounds().contains(mousePos))
					{
						if (difficulty == EASY)
						{
							if (shuffleCounter < MAX_SHUFFLE_EASY)
							{
								ShufflePlayerTiles(playerTiles);
							}
							else
							{
								playerShuffleButton.setColor(sf::Color::Red);
							}
						}
						else if (difficulty == MEDIUM)
						{
							if (shuffleCounter < MAX_SHUFFLE_MEDIUM)
							{
								ShufflePlayerTiles(playerTiles);
							}
							else
							{
								playerShuffleButton.setColor(sf::Color::Red);
							}
						}
						else if (difficulty == HARD)
						{
							if (shuffleCounter < MAX_SHUFFLE_HARD)
							{
								ShufflePlayerTiles(playerTiles);
							}
							else
							{
								playerShuffleButton.setColor(sf::Color::Red);
							}
						}
					}
				}
			}
			if (endTurnButton.getGlobalBounds().contains(mousePos))
			{
				NextTurn();
			}
			if (bagButton.getGlobalBounds().contains(mousePos))
			{
				isBagOpen = !isBagOpen;
			}
		}
	}

}

/// <summary>
/// Update loop has all the functions depending on if its the players turn or the AI's turn
/// </summary>
/// <param name="t_deltaTime"></param>
void Game::update(sf::Time t_deltaTime)
{
	
	if (m_exitGame)	{m_window.close();}

	if (state == MENU)
	{
		processEvents();
	}
	else if (state == GAME)
	{

		aiTimer = aiClock.getElapsedTime();
		qwirkleTimer = qwirkleClock.getElapsedTime();

		playerScoreText.setString(std::to_string(playerScore));
		aiScoreText.setString(std::to_string(aiScore));

		if (qwirkleTimer.asSeconds() > 2)
		{
			SetQwirkleText("");
		}
		else
		{
			qwirkleText.setScale(qwirkleTimer.asSeconds(), qwirkleTimer.asSeconds());
		}

		if (isPlayerTurn)
		{
			processEvents();
			playerNameText.setFillColor(sf::Color::Green);
			aiNameText.setFillColor(sf::Color::White);
		}
		else
		{
			playerNameText.setFillColor(sf::Color::White);
			aiNameText.setFillColor(sf::Color::Green);
			if (isSinglePlayer)
			{
				if (aiTimer.asSeconds() >= 2)
				{

					SortAiTurn();
				}
			}
			else
			{
				processEvents();
			}

		}
	}

}

/// <summary>
/// Draws everything on the screen
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color(0, 0, 255, 100));
	m_window.draw(easyButton);

	if (state == MENU)
	{
		m_window.draw(qwirkleHeadingText);
		m_window.draw(easyButton);
		m_window.draw(easyText);
		m_window.draw(mediumButton);
		m_window.draw(mediumText);
		m_window.draw(hardButton);
		m_window.draw(hardText);

		m_window.draw(pveButton);
		m_window.draw(pveText);

		m_window.draw(pvpButton);
		m_window.draw(pvpText);

	}
	else if (state == GAME)
	{
		m_window.draw(tableSprite);

		for (int row = 0; row < Global::ROWS_COLUMNS; row++)
		{
			for (int col = 0; col < Global::ROWS_COLUMNS; col++)
			{
				board[row][col].Render(m_window);
			}
		}

		for (int i = 0; i < 6; i++)
		{
			playerTiles[i].Render(m_window);
		}

		for (int i = 0; i < 6; i++)
		{
			aiTiles[i].Render(m_window);
		}

		if (isBagOpen)
		{
			m_window.draw(bagRect);
			for (int i = 0; i < 108; i++)
			{
				totalTilePool[i].Render(m_window);
			}
		}
		m_window.draw(playerShuffleButton);
		m_window.draw(endTurnButton);
		m_window.draw(bagButton);
		m_window.draw(playerScoreText);
		m_window.draw(aiScoreText);
		m_window.draw(playerNameText);
		m_window.draw(aiNameText);
		m_window.draw(qwirkleText);
		
	}

	m_window.display();
}

/// <summary>
/// Initilaises everything 
/// </summary>
void Game::init()
{
	state = MENU;
	isSinglePlayer = true;
	srand(time(nullptr));
	tileTexture.loadFromFile("ASSETS\\IMAGES\\tile.png");
	insideBagTexture.loadFromFile("ASSETS\\IMAGES\\leather.png");

	InitBoard();
	SetupTilePool();
	SetupPlayerTiles();
	SetupAITiles();
	SetupSprites();
	
}

/// <summary>
/// Sets up the board for the tiles to be placed on 
/// </summary>
void Game::InitBoard()
{
	// BOARD
	board = new Tile * [Global::ROWS_COLUMNS];

	for (int i = 0; i < Global::ROWS_COLUMNS; i++)
	{
		board[i] = new Tile[Global::ROWS_COLUMNS];
	}

	for (int row = 0; row < Global::ROWS_COLUMNS; row++)
	{
		for (int col = 0; col < Global::ROWS_COLUMNS; col++)
		{
			board[row][col].Init(sf::Vector2f(row * Global::CELL_SIZE, col * Global::CELL_SIZE));
		}
	}
}

/// <summary>
/// Sets up the bag of tiles where the player and AI get the tiles from
/// </summary>
void Game::SetupTilePool()
{
	for (int i = 0; i < 36; i++)
	{
		float xPosition = (i % 6) * Global::CELL_SIZE + Global::S_WIDTH * 0.35;
		float yPosition = Global::S_HEIGHT * 0.05 + (i / 6) * Global::CELL_SIZE;

		tilePool1[i].Init(sf::Vector2f(xPosition, yPosition));
		tilePool1[i].tile.setFillColor(sf::Color::White);
		tilePool1[i].tile.setTexture(&tileTexture);
		tilePool1[i].SetPiece(i);
	}

	for (int i = 0; i < 36; i++)
	{
		float xPosition = (i % 6) * Global::CELL_SIZE + Global::S_WIDTH * 0.35;
		float yPosition = Global::S_HEIGHT * 0.05 + (i / 6) * Global::CELL_SIZE + Global::CELL_SIZE * 6;

		tilePool2[i].Init(sf::Vector2f(xPosition, yPosition));
		tilePool2[i].tile.setFillColor(sf::Color::White);
		tilePool2[i].tile.setTexture(&tileTexture);
		tilePool2[i].SetPiece(i);
	}

	for (int i = 0; i < 36; i++)
	{
		float xPosition = (i % 6) * Global::CELL_SIZE + Global::S_WIDTH * 0.35;
		float yPosition = Global::S_HEIGHT * 0.05 + (i / 6) * Global::CELL_SIZE + Global::CELL_SIZE * 12;

		tilePool3[i].Init(sf::Vector2f(xPosition, yPosition));
		tilePool3[i].tile.setFillColor(sf::Color::White);
		tilePool3[i].tile.setTexture(&tileTexture);
		tilePool3[i].SetPiece(i);
	}

	std::copy(tilePool1, tilePool1 + 36, totalTilePool);
	std::copy(tilePool2, tilePool2 + 36, totalTilePool + 36);
	std::copy(tilePool3, tilePool3 + 36, totalTilePool + 72);

	bagRect.setPosition( Global::S_WIDTH * 0.3, 0);
	bagRect.setTexture(&insideBagTexture);
	bagRect.setSize(sf::Vector2f(Global::CELL_SIZE * 8, Global::CELL_SIZE * 20));
}

/// <summary>
/// Uses that tilepool to assign 6 random tiles to the player
/// </summary>
void Game::SetupPlayerTiles()
{
	int count = 0;
	while (count < 6)
	{
		playerTiles[count].Init(sf::Vector2f((count * Global::CELL_SIZE) + Global::S_HEIGHT * 0.1, Global::S_HEIGHT * 1.01));
		int randomTileFromTilePool = rand() % 108;
		if (!totalTilePool[randomTileFromTilePool].GetUsed())
		{
			playerTiles[count].tile.setFillColor(sf::Color::White);
			playerTiles[count].tile.setTexture(&tileTexture);
			playerTiles[count].SetPiece(totalTilePool[randomTileFromTilePool].GetCurrentPiece());
			totalTilePool[randomTileFromTilePool].SetUsed();
			count++;
		}
	}
}

/// <summary>
/// Uses that tilepool to assign 6 random tiles to the AI
/// </summary>
void Game::SetupAITiles()
{
	int count = 0;
	while (count < 6)
	{
		aiTiles[count].Init(sf::Vector2f((count * Global::CELL_SIZE) + Global::S_HEIGHT * 0.6, Global::S_HEIGHT * 1.01));
		int randomTileFromTilePool = rand() % 108;
		if (!totalTilePool[randomTileFromTilePool].GetUsed())
		{
			aiTiles[count].tile.setFillColor(sf::Color::White);
			aiTiles[count].tile.setTexture(&tileTexture);
			aiTiles[count].SetPiece(totalTilePool[randomTileFromTilePool].GetCurrentPiece());
			totalTilePool[randomTileFromTilePool].SetUsed();
			count++;
		}
	}
}

/// <summary>
/// Sets up all the shapes, sprites and fonts 
/// </summary>
void Game::SetupSprites()
{
	shuffleTexture.loadFromFile("ASSETS\\IMAGES\\shuffle.png");
	endTurnTexture.loadFromFile("ASSETS\\IMAGES\\turn.png");
	bagTexture.loadFromFile("ASSETS\\IMAGES\\bag.png");
	tableTexture.loadFromFile("ASSETS\\IMAGES\\table.png");
	playerNameTexture.loadFromFile("ASSETS\\IMAGES\\playerName.png");
	font.loadFromFile("ASSETS\\FONTS\\ariblk.ttf");

	// MENU

		// Buttons
	qwirkleHeadingText.setFont(font);
	qwirkleHeadingText.setString("Qwirkle");
	qwirkleHeadingText.setCharacterSize(50);
	qwirkleHeadingText.setPosition(Global::S_WIDTH / 2, Global::S_HEIGHT / 5);
	qwirkleHeadingText.setOrigin(qwirkleHeadingText.getLocalBounds().width / 2, (qwirkleHeadingText.getLocalBounds().height / 2) + 5);
	qwirkleHeadingText.setFillColor(sf::Color::Black);
	qwirkleHeadingText.setLetterSpacing(2);

	easyButton.setSize(sf::Vector2f{ 300, 50 });
	easyButton.setPosition(Global::S_WIDTH / 2, Global::S_HEIGHT / 3);
	easyButton.setOrigin(easyButton.getLocalBounds().width / 2, easyButton.getLocalBounds().height / 2);
	easyButton.setFillColor(sf::Color::Green);

	mediumButton.setSize(sf::Vector2f{ 300, 50 });
	mediumButton.setPosition(Global::S_WIDTH / 2, Global::S_HEIGHT / 2);
	mediumButton.setOrigin(easyButton.getLocalBounds().width / 2, easyButton.getLocalBounds().height / 2);
	mediumButton.setFillColor(sf::Color(252, 186, 3));


	hardButton.setSize(sf::Vector2f{ 300, 50 });
	hardButton.setPosition(Global::S_WIDTH / 2, Global::S_HEIGHT / 1.5);
	hardButton.setOrigin(hardButton.getLocalBounds().width / 2, hardButton.getLocalBounds().height / 2);
	hardButton.setFillColor(sf::Color::Red);

	pveButton.setRadius(80);
	pveButton.setPosition(Global::S_WIDTH * 0.2, Global::S_HEIGHT * 0.9);
	pveButton.setOrigin(pveButton.getRadius(), pveButton.getRadius());
	pveButton.setFillColor(sf::Color::Green);
	pveButton.setOutlineThickness(1);
	pveButton.setOutlineColor(sf::Color::Black);

	pvpButton.setRadius(80);
	pvpButton.setPosition(Global::S_WIDTH * 0.8, Global::S_HEIGHT * 0.9);
	pvpButton.setOrigin(pvpButton.getRadius(), pvpButton.getRadius());
	pvpButton.setFillColor(sf::Color::Red);
	pvpButton.setOutlineThickness(1);
	pvpButton.setOutlineColor(sf::Color::Black);

	// Text
	easyText.setFont(font);
	easyText.setString("Easy");
	easyText.setCharacterSize(30);
	easyText.setPosition(easyButton.getPosition());
	easyText.setOrigin(easyText.getLocalBounds().width / 2, (easyText.getLocalBounds().height / 2) + 5);
	easyText.setFillColor(sf::Color::Black);
	easyText.setLetterSpacing(2);

	mediumText.setFont(font);
	mediumText.setString("Medium");
	mediumText.setCharacterSize(30);
	mediumText.setPosition(mediumButton.getPosition());
	mediumText.setOrigin(mediumText.getLocalBounds().width / 2, (mediumText.getLocalBounds().height / 2) + 5);
	mediumText.setFillColor(sf::Color::Black);

	hardText.setFont(font);
	hardText.setString("Hard");
	hardText.setCharacterSize(30);
	hardText.setPosition(hardButton.getPosition());
	hardText.setOrigin(hardText.getLocalBounds().width / 2, (hardText.getLocalBounds().height / 2) + 5);
	hardText.setFillColor(sf::Color::Black);


	// GAME
	playerShuffleButton.setTexture(shuffleTexture);
	playerShuffleButton.setScale(0.2, 0.2);
	playerShuffleButton.setPosition(playerTiles[5].tile.getPosition().x + Global::CELL_SIZE / 2, playerTiles[0].tile.getPosition().y + 45);
	
	endTurnButton.setTexture(endTurnTexture);
	endTurnButton.setScale(0.2, 0.2);
	endTurnButton.setPosition(aiTiles[0].tile.getPosition().x - Global::CELL_SIZE / 2, aiTiles[0].tile.getPosition().y + 45);

	bagButton.setTexture(bagTexture);
	bagButton.setScale(0.2, 0.2);
	bagButton.setOrigin(bagButton.getLocalBounds().width / 2, bagButton.getLocalBounds().height / 2);
	bagButton.setPosition(Global::S_WIDTH * 0.5, playerTiles[0].tile.getPosition().y + 25);

	tableSprite.setTexture(tableTexture);
	tableSprite.setScale(static_cast<float>(Global::S_WIDTH) / tableTexture.getSize().x, static_cast<float>(Global::S_HEIGHT) / tableTexture.getSize().y);

	playerNameText.setFont(font);
	playerNameText.setString("Player");
	playerNameText.setOutlineThickness(1);
	playerNameText.setOutlineColor(sf::Color::Black);
	playerNameText.setCharacterSize(26);
	playerNameText.setPosition(playerTiles[2].tile.getPosition().x, playerTiles[0].tile.getPosition().y + 50);

	aiNameText.setFont(font);
	aiNameText.setString("AI");
	aiNameText.setOutlineThickness(1);
	aiNameText.setOutlineColor(sf::Color::Black);
	aiNameText.setCharacterSize(26);
	aiNameText.setPosition(aiTiles[3].tile.getPosition().x, aiTiles[0].tile.getPosition().y + 50);

	playerScoreText.setFont(font);
	playerScoreText.setString(std::to_string(playerScore));
	playerScoreText.setCharacterSize(26);
	playerScoreText.setPosition(playerTiles[5].tile.getPosition().x + Global::CELL_SIZE * 1.3, playerTiles[0].tile.getPosition().y + 5);

	aiScoreText.setFont(font);
	aiScoreText.setString(std::to_string(aiScore));
	aiScoreText.setCharacterSize(26);
	aiScoreText.setPosition(aiTiles[0].tile.getPosition().x - Global::CELL_SIZE * 0.7, aiTiles[0].tile.getPosition().y + 5);

	qwirkleText.setFont(font);
	qwirkleText.setString(qwirkleString);
	qwirkleText.setOrigin(qwirkleText.getLocalBounds().width / 2, qwirkleText.getLocalBounds().height /2);
	qwirkleText.setCharacterSize(50);
	qwirkleText.setPosition(Global::S_WIDTH / 2, Global::S_HEIGHT / 2);

	pveText.setFont(font);
	pveText.setString("AI");
	pveText.setCharacterSize(50);
	pveText.setPosition(pveButton.getPosition());
	pveText.setOrigin(pveText.getLocalBounds().width / 2, (pveText.getLocalBounds().height / 2) + 5);
	pveText.setFillColor(sf::Color::Black);

	pvpText.setFont(font);
	pvpText.setString("Multiplayer");
	pvpText.setCharacterSize(30);
	pvpText.setPosition(pvpButton.getPosition());
	pvpText.setOrigin(pvpText.getLocalBounds().width / 2, (pvpText.getLocalBounds().height / 2) + 5);
	pvpText.setFillColor(sf::Color::Black);
}

/// <summary>
/// Looks through the tile pool and picks a random number as long as it hasnt been used already
/// </summary>
/// <returns>Random number of an active tile in the bag</returns>
int Game::GetActiveTilePool()
{
	int random = rand() % 108;

	if (!totalTilePool[random].isUsed)
	{
		int count = 0;
		for each (Tile tile in totalTilePool)
		{
			if (tile.GetCurrentPiece() == totalTilePool[random].GetCurrentPiece() && !tile.isUsed)
			{
				count++;
			}
		}
		return random;
	}
	else
	{
		GetActiveTilePool();
	}

}

/// <summary>
/// Starts the game, decides who goes first or if its singe player or multiplayer
/// </summary>
void Game::StartGame()
{
	if (isSinglePlayer)
	{
		if (CheckWhoGoesFirst(playerTiles) > CheckWhoGoesFirst(aiTiles))
		{
			isPlayerTurn = true;
			_Output_To_Screen("Player goes first");
		}
		else if (CheckWhoGoesFirst(playerTiles) < CheckWhoGoesFirst(aiTiles))
		{
			isPlayerTurn = false;
			_Output_To_Screen("AI goes first");
		}
		else
		{
			int random = rand() % 2;

			_Output_To_Screen("Draw, random time....");
			if (random == 0)
			{
				isPlayerTurn = true;
				_Output_To_Screen("Player goes first");
			}
			else
			{
				isPlayerTurn = false;
				_Output_To_Screen("AI goes first");
			}
		}
	}
	else
	{
		playerNameText.setString("Player 1");
		aiNameText.setString("Player 2");

		if (CheckWhoGoesFirst(playerTiles) > CheckWhoGoesFirst(aiTiles))
		{
			isPlayerTurn = true;
			_Output_To_Screen("Player1 goes first");
		}
		else if (CheckWhoGoesFirst(playerTiles) < CheckWhoGoesFirst(aiTiles))
		{
			isPlayerTurn = false;
			_Output_To_Screen("Player 2 goes first");
		}
		else
		{
			int random = rand() % 2;

			_Output_To_Screen("Draw, random time....");
			if (random == 0)
			{
				isPlayerTurn = true;
				_Output_To_Screen("Player 1 goes first");
			}
			else
			{
				isPlayerTurn = false;
				_Output_To_Screen("Player 2 goes first");
			}
		}
		
	}
	
}

/// <summary>
/// Checks the playersTiles and AITiles, it sees which one has the most common shapes and colours
/// </summary>
/// <param name="t_tiles">Takes in an array of tiles for the player or AI</param>
/// <returns>Returns the count of the most common colour or shape</returns>
int Game::CheckWhoGoesFirst(Tile t_tiles[6])
{
	std::map<std::string, int> shapeCount;
	std::map<std::string, int> colorCount;

	int maxS = 0;
	for (int i = 0; i < 6; i++)
	{
		shapeCount[t_tiles[i].tileShape]++;
		colorCount[t_tiles[i].tileColor]++;
	}

	std::string maxShape;
	int maxShapeCount = 0;
	for (const auto& pair : shapeCount)
	{
		if (pair.second > maxShapeCount)
		{
			maxShape = pair.first;
			maxShapeCount = pair.second;
		}
	}

	std::string maxColor;
	int maxColorCount = 0;
	for (const auto& pair : colorCount)
	{
		if (pair.second > maxColorCount)
		{
			maxColor = pair.first;
			maxColorCount = pair.second;
		}
	}

	if (maxShapeCount > maxColorCount)
	{
		return maxShapeCount;
	}
	else if (maxColorCount > 0)
	{
		return maxColorCount;

	}
}

/// <summary>
/// Checks which player tile the player has selected
/// </summary>
/// <param name="tiles">Takes in an array of tiles from the player or AI</param>
void Game::SelectPlayerTile(Tile tiles[6])
{
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(Global::GetMousePos(m_window));

	for (int i = 0; i < 6; i++)
	{
		sf::FloatRect bounds = tiles[i].shape.getGlobalBounds();

		if (bounds.contains(mousePos))
		{

			if (!tiles[i].GetUsed())
			{
				// Delsecect old tile
				tiles[selectedTile].DeselectTile();
				// Select new tile
				tiles[i].SelectTile();
				// Get a new selected tile
				selectedTile = i;
				// Get the shape / piece from the tile you selected
				currentSelectedPiece = tiles[i].GetCurrentPiece();
				tiles[i].CheckPiece(currentSelectedPiece);
			}
		}
	}
}

/// <summary>
/// Using the selected tile, it places the tile on the game board
/// </summary>
/// <param name="tiles">Takes in an array of tiles from the player or AI</param>
void Game::PlaceTileOnBoard(Tile tiles[6])
{
	sf::Vector2f mousePos = static_cast<sf::Vector2f>(Global::GetMousePos(m_window));

	for (int row = 0; row < Global::ROWS_COLUMNS; row++)
	{
		for (int col = 0; col < Global::ROWS_COLUMNS; col++)
		{
			sf::FloatRect bounds = board[row][col].shape.getGlobalBounds();

			if (bounds.contains(mousePos))
			{
				currentCellPos = { row, col };
				std::cout << "Row: " << row << ", " << col << "\n";

				isValidPlacement = CheckValidTileColorOrShape(GetValidNeighbours(row, col), tiles);
				if (isValidPlacement && movesInTurnCount != 0)
				{
					if (row == CheckIfPlacingInSameLine(row, col) || col == CheckIfPlacingInSameLine(row, col))
					{
						isValidPlacement = true;
					}
					else
					{
						isValidPlacement = false;
					}
				}

				if (isValidPlacement || isFirstMoveOfGame)
				{

					previousPlacedTile = { row, col };
					board[row][col].SetPiece(currentSelectedPiece);
					SetTurnColourAndShape(tiles[selectedTile].GetCurrentColor(), tiles[selectedTile].GetCurrentShape());
					tiles[selectedTile].DeselectTile();
					tiles[selectedTile].SetUsed();
					std::cout << "You have placed : " << board[row][col].tileName << "\n";
					board[row][col].tile.setFillColor(sf::Color::White);
					board[row][col].tile.setTexture(&tileTexture);

					if (movesInTurnCount == 0)
					{
						firstTilePlacedInTurn = board[row][col];
						sameLineVector = { row, col };
					}

					tilesPlacedInTurnForScore.push_back(board[row][col]);
					movesInTurnCount++;
					isFirstMoveOfGame = false;

				}
			}
		}

	}
}
/// <summary>
/// Takes in the row and col from the board
/// Checks if the tile you are trying to place matches the first tile you placed in your turn
/// This stops you from placing tiles in the same line
/// </summary>
/// <param name="row">Checks the row on the board</param>
/// <param name="col">Checks the col on the board</param>
/// <returns>The number the </returns>
int Game::CheckIfPlacingInSameLine(int row, int col)
{
	if (movesInTurnCount == 1)
	{
		if (sameLineVector.x == row)
		{
			currentLineNumber = row;
		}
		else if (sameLineVector.y == col)
		{
			currentLineNumber = col;
		}
		else
		{
			_Output_To_Screen("The tile you are trying to place is not part of the same line");
			return -1;
		}
	}

	if (row == currentLineNumber)
	{
		for (int i = 1; i < 6; i++)
		{
			int sameLineCountDown = sameLineVector.y + i;
			int sameLineCountUp = sameLineVector.y - i;

			if (sameLineCountDown <= Global::ROWS_COLUMNS && 
				sameLineVector.y < col)
			{
				if (board[row][sameLineCountDown].GetCurrentColor() == EMPTY_C && 
					sameLineCountDown < col)
				{
					_Output_To_Screen("The tile you are trying to place is not part of the same line");
					return -1;
				}
			}
			if (sameLineCountUp >= 0 &&
				sameLineVector.y > col)
			{
				if (board[row][sameLineCountUp].GetCurrentColor() == EMPTY_C && 
					sameLineCountUp > col)
				{
					_Output_To_Screen("The tile you are trying to place is not part of the same line");
					return -1;
				}
			}
		}
	}

	if (col == currentLineNumber)
	{
		for (int i = 1; i < 6; i++)
		{
			int sameLineCountRight = sameLineVector.x + i;
			int sameLineCountLeft = sameLineVector.x - i;

			if (sameLineCountRight <= Global::ROWS_COLUMNS && sameLineVector.x < row)
			{
				if (board[sameLineCountRight][col].GetCurrentColor() == EMPTY_C && sameLineCountRight < row)
				{
					_Output_To_Screen("The tile you are trying to place is not part of the same line");
					return -1;
				}
			}
			if (sameLineCountLeft >= 0 && sameLineVector.x > row)
			{
				if (board[sameLineCountLeft][col].GetCurrentColor() == EMPTY_C && sameLineCountLeft > row)
				{
					_Output_To_Screen("The tile you are trying to place is not part of the same line");
					return -1;
				}
			}
		}
	}

	return currentLineNumber;
}

/// <summary>
/// Puts the players tiles back in the bag and takes out 6 more ones
/// </summary>
/// <param name="tiles">Takes in an array of tiles from the player or AI<</param>
void Game::ShufflePlayerTiles(Tile tiles[6])
{
	shuffleCounter++;

	for (int i = 0; i < 6; i++)
	{
		
		totalTilePool[playerTileIdsForBag[i]].ResetTile();
		tiles[i].SetUsed();
		tiles[i].DeselectTile();
	}
	RefillPlayerAndAITiles();
}
/// <summary>
/// Sets the board tiles colour and shape of the tile you have selected
/// </summary>
/// <param name="color">Colour of the current tile selected</param>
/// <param name="shape">Shape of the current tile selected</param>
void Game::SetTurnColourAndShape(Color color, Shape shape)
{
	if (turnColor == EMPTY_C)
	{
		turnColor = color;
	}
	if (turnShape == EMPTY_S)
	{
		turnShape = shape;
	}
}

/// <summary>
/// Handles all the logic for when the current turn ends
/// </summary>
void Game::NextTurn()
{
	RefillPlayerAndAITiles();
	if(isPlayerTurn)
	{
		playerScore += AddScoresForTurn(tilesPlacedInTurnForScore);
	}
	else
	{
		aiScore += AddScoresForTurn(tilesPlacedInTurnForScore);
	}
	shuffleCounter = 0;
	playerShuffleButton.setColor(sf::Color::White);
	tilesPlacedInTurnForScore.clear();
	movesInTurnCount = 0;
	isColorTurn = false;
	isShapeTurn = false;
	turnColor = EMPTY_C;
	turnShape = EMPTY_S;
	currentTurn++;
	isPlayerTurn = !isPlayerTurn;
	aiClock.restart();
}

/// <summary>
/// At the end of each turn, refill all the tiles back to 6
/// </summary>
void Game::RefillPlayerAndAITiles()
{
	for (int i = 0; i < 6; i++)
	{
		if (playerTiles[i].GetUsed())
		{
			playerTiles[i].ResetTile();
			int randomTileFromTilePool = GetActiveTilePool();
			// Refilling the players tile with the random piece
			playerTiles[i].SetPiece(randomTileFromTilePool);
			// Removing the tile from the tile pool
			totalTilePool[randomTileFromTilePool].SetUsed();
			playerTileIdsForBag[i] = randomTileFromTilePool;
		}
	}

	for (int i = 0; i < 6; i++)
	{
		if (aiTiles[i].GetUsed())
		{
			aiTiles[i].ResetTile();
			int randomTileFromTilePool = GetActiveTilePool();
			// Refilling the players tile with the random piece
			aiTiles[i].SetPiece(randomTileFromTilePool);
			// Removing the tile from the tile pool
			totalTilePool[randomTileFromTilePool].SetUsed();
			aiTileIdsForBag[i] = randomTileFromTilePool;
		}
	}
}

/// <summary>
/// Takes a tile and gets its position on the board
/// </summary>
/// <param name="tile"Takes a tile from the board></param>
/// <returns>A vector2i with the position of the tile on the board</returns>
sf::Vector2i Game::GetCellPosFromTile(Tile tile)
{
	sf::Vector2i tilePos;

	tilePos.x = tile.tile.getPosition().x / Global::CELL_SIZE;
	tilePos.y = tile.tile.getPosition().y / Global::CELL_SIZE;
	return tilePos;
}
/// <summary>
/// Takes a vector of tiles and gets its position on the board
/// </summary>
/// <param name="tiles">Takes a vector of tiles</param>
/// <returns>A vector of vector2i with the positions on the board</returns>
std::vector<sf::Vector2i> Game::GetCellPosFromTileList(std::vector<Tile> tiles)
{
	std::vector<sf::Vector2i> tilePosVector;
	sf::Vector2i tilePos;

	for each (Tile tile in tiles)
	{
		tilePos.x = tile.tile.getPosition().x / Global::CELL_SIZE;
		tilePos.y = tile.tile.getPosition().y / Global::CELL_SIZE;

		tilePosVector.push_back(tilePos);
	}


	return tilePosVector;
}

/// <summary>
/// Checks the current tiles, neighbours and checks to see if they arent empty
/// </summary>
/// <param name="row">Checks the row on the board</param>
/// <param name="col">Checks the col on the board</param>
/// <returns>Vector of tiles that are all the valid, non empty tiles</returns>
std::vector<Tile> Game::GetValidNeighbours(int row, int col)
{
	std::vector<Tile> neighbourTiles;
	std::vector<Tile> validNeighbourTiles;

	for (int i = row - 1; i <= row + 1; ++i)
	{
		for (int j = col - 1; j <= col + 1; ++j)
		{
			if ((i == row && j == col) || (i != row && j != col)) { continue; }

			if (i >= 0 && i < Global::ROWS_COLUMNS
				&& j >= 0 && j < Global::ROWS_COLUMNS)
			{
				neighbourTiles.push_back(board[i][j]);
			}
		}
	}

	for each (auto tile in neighbourTiles)
	{
		if (tile.GetCurrentColor() != EMPTY_C || tile.GetCurrentShape() != EMPTY_S)
		{
			validNeighbourTiles.push_back(tile);
		}
	}

	return validNeighbourTiles;
}

/// <summary>
/// Checks to see if the tile you are placing can be placed by checking the valid neighbours 
/// </summary>
/// <param name="validNeighbours">The vector of non empty tiles that are the neighbours of the tile you are trying to place</param>
/// <param name="tiles">Takes in the selected tile for the player or AI tiles</param>
/// <returns>The bool to see if its a valid move or not</returns>
bool Game::CheckValidTileColorOrShape(std::vector<Tile> validNeighbours, Tile tiles[6])
{
	bool isValid = false;
	Tile selectedPlayerTile = tiles[selectedTile];

	for each (auto tile in validNeighbours)
	{
		if (selectedPlayerTile.GetCurrentColor() == tile.GetCurrentColor() || selectedPlayerTile.GetCurrentShape() == tile.GetCurrentShape())
		{
			isValid = CheckFurtherInLine(selectedPlayerTile, tile);
		}
		else
		{
			isValid = false;
			return isValid;
		}
	}
	return isValid;
}

/// <summary>
/// This checks that the tile you are trying to place is in a valid line
/// </summary>
/// <param name="playerTile">Tile you want to place</param>
/// <param name="validTile">Tile you are comparing it to</param>
/// <returns>A bool to see if its valid</returns>
bool Game::CheckFurtherInLine(Tile playerTile, Tile validTile)
{
	bool isValid = false;
	sf::Vector2i playerCell = currentCellPos;
	sf::Vector2i validTileCell = GetCellPosFromTile(validTile);

	int leftOfValidTile = validTileCell.x - 1;
	int rightOfValidTile = validTileCell.x + 1;
	int aboveValidTile = validTileCell.y - 1;
	int belowValidTile = validTileCell.y + 1;

	leftOfValidTile = (leftOfValidTile < 0) ? 0 : leftOfValidTile;
	rightOfValidTile = (rightOfValidTile > Global::ROWS_COLUMNS) ? Global::ROWS_COLUMNS : rightOfValidTile;
	aboveValidTile = (aboveValidTile < 0) ? 0 : aboveValidTile;
	belowValidTile = (belowValidTile > Global::ROWS_COLUMNS) ? Global::ROWS_COLUMNS : belowValidTile;

	if (playerCell.x == validTileCell.x)
	{
		if (validTileCell.y < playerCell.y)
		{
			// CHECK IN A LINE GOING UP, IF THE COLORS MATCH. STOPS WHEN IT FINDS AN EMPTY TILE
			for (int i = 0; validTileCell.y - i >= 0 && board[validTileCell.x][validTileCell.y - i].GetCurrentColor() != EMPTY_C; i++)
			{
				//validTileCell.y = (validTileCell.y - i < 0) ? 0 : validTileCell.y;
				int tilesInLineCountX = validTileCell.x - i;
				int tilesInLineCountY = validTileCell.y - i;
				
				// CANT HAVE THE EXACT SAME TILE IN THE SAME LINE
				if (board[validTileCell.x][tilesInLineCountY].GetCurrentPiece() == playerTile.GetCurrentPiece())
				{
					_Output_To_Screen("Warning: Cant place two of the same tiles in the same line");

					isValid = false;
					return isValid;
				}
				else
				{
					if (board[validTileCell.x][tilesInLineCountY].GetCurrentColor() == playerTile.GetCurrentColor())
					{
						isValid = true;
					}
					else if (board[validTileCell.x][tilesInLineCountY].GetCurrentShape() == playerTile.GetCurrentShape())
					{
						isValid = true;
					}
					else
					{
						isValid = false;
						return isValid;
					}
				}
			}
			 if (board[validTileCell.x][aboveValidTile].GetCurrentShape() == EMPTY_S ||
				board[validTileCell.x][aboveValidTile].GetCurrentColor() == EMPTY_C)
			{
				isValid = true;
			}

		}
		else if (validTileCell.y > playerCell.y)
		{
			for (int i = 0; validTileCell.y + i < Global::ROWS_COLUMNS && board[validTileCell.x][validTileCell.y + i].GetCurrentColor() != EMPTY_C; i++)
			{
				if (board[validTileCell.x][validTileCell.y + i].GetCurrentPiece() == playerTile.GetCurrentPiece())
				{
					_Output_To_Screen("Warning: Cant place two of the same tiles in the same line");

					isValid = false;
					return isValid;
				}
				else
				{
					if (board[validTileCell.x][validTileCell.y + i].GetCurrentColor() == playerTile.GetCurrentColor())
					{
						isValid = true;
					}
					else if (board[validTileCell.x][validTileCell.y + i].GetCurrentShape() == playerTile.GetCurrentShape())
					{
						isValid = true;
					}
					else
					{
						isValid = false;
						return isValid;
					}

				}
			}
			if (board[validTileCell.x][belowValidTile].GetCurrentShape() == EMPTY_S ||
				board[validTileCell.x][belowValidTile].GetCurrentColor() == EMPTY_C)
			{
				isValid = true;
			}
		}
	}
	if (playerCell.y == validTileCell.y)
	{
		if (validTileCell.x < playerCell.x)
		{
			for (int i = 0; validTileCell.x - i >= 0 && board[validTileCell.x - i][validTileCell.y].GetCurrentColor() != EMPTY_C; i++)
			{
				if (board[validTileCell.x - i][validTileCell.y].GetCurrentPiece() == playerTile.GetCurrentPiece())
				{
					_Output_To_Screen("Warning: Cant place two of the same tiles in the same line");

					isValid = false;
					return isValid;
				}
				else
				{
					if (board[validTileCell.x - i][validTileCell.y].GetCurrentColor() == playerTile.GetCurrentColor())
					{
						isValid = true;
					}
					else if (board[validTileCell.x - i][validTileCell.y].GetCurrentShape() == playerTile.GetCurrentShape())
					{
						isValid = true;
					}
					else
					{
						isValid = false;
						return isValid;
					}
				}
			}
		}
		else if (validTileCell.x > playerCell.x)
		{
			for (int i = 0; validTileCell.x + i < Global::ROWS_COLUMNS && board[validTileCell.x + i][validTileCell.y].GetCurrentColor() != EMPTY_C; i++)
			{
				if (board[validTileCell.x + i][validTileCell.y].GetCurrentPiece() == playerTile.GetCurrentPiece())
				{
					_Output_To_Screen("Warning: Cant place two of the same tiles in the same line");

					isValid = false;
					return isValid;
				}
				else
				{
					if (board[validTileCell.x + i][validTileCell.y].GetCurrentColor() == playerTile.GetCurrentColor())
					{
						isValid = true;
					}
					else if (board[validTileCell.x + i][validTileCell.y].GetCurrentShape() == playerTile.GetCurrentShape())
					{
						isValid = true;
					}
					else
					{
						isValid = false;
						return isValid;
					}
				}
			}
		}
	}

	return isValid;
}

/// <summary>
/// Uses a map, loops through all the tiles connected to the tile you placed
/// Adds that tile to the map and then increases the count
/// If you get a Qwirkle, add on 6 extra points
/// </summary>
/// <param name="tilesToAddUp">Vector of tiles that you want to use to get the score</param>
/// <returns>The size of the map</returns>
int Game::AddScoresForTurn(std::vector<Tile> tilesToAddUp)
{
	int counter = 0;

	std::vector<Tile> connectedTiles;

	std::map<Tile, int> tileMap;
	std::map<sf::Vector2i, int> posMap;
	//posMap[{1, 1}]++; ///// ERROR

	int qwirkleCount = 0;

	bool qwirkle = false;

	for each (Tile tile in tilesToAddUp)
	{
		sf::Vector2i tilePos = GetCellPosFromTile(tile);

		int leftCount = 0;
		int rightCount = 0;
		int upCount = 0;
		int downCount = 0;
		// RIGHT OF TILE UNTIL IT FINDS EMPTY TILE
		for (int i = 1; tilePos.x + i < Global::ROWS_COLUMNS && board[tilePos.x + i][tilePos.y].GetCurrentColor() != EMPTY_C; i++)
		{
			tileMap[board[tilePos.x + i][tilePos.y]]++;
			//posMap[GetCellPosFromTile(board[tilePos.x + i][tilePos.y])]++;
			connectedTiles.push_back(board[tilePos.x + i][tilePos.y]);
			rightCount++;
		}
		// LEFT OF TILE UNTIL IT FINDS EMPTY TILE
		for (int i = 1; tilePos.x - i > 0 && board[tilePos.x - i][tilePos.y].GetCurrentColor() != EMPTY_C; i++)
		{
			tileMap[board[tilePos.x - i][tilePos.y]]++;
			//posMap[GetCellPosFromTile(board[tilePos.x - i][tilePos.y])]++;
			connectedTiles.push_back(board[tilePos.x - i][tilePos.y]);
			leftCount++;
		}
		// ABOVE TILE UNTIL IT FINDS EMPTY TILE
		for (int i = 1; tilePos.y - i > 0 && board[tilePos.x][tilePos.y - i].GetCurrentColor() != EMPTY_C; i++)
		{
			tileMap[board[tilePos.x][tilePos.y - i]]++;

			//posMap[GetCellPosFromTile(board[tilePos.x][tilePos.y - i])]++;
			connectedTiles.push_back(board[tilePos.x][tilePos.y - i]);
			upCount++;
		}
		// BELOW TILE UNTIL IT FINDS EMPTY TILE
		for (int i = 1; tilePos.y + i < Global::ROWS_COLUMNS && board[tilePos.x][tilePos.y + i].GetCurrentColor() != EMPTY_C; i++)
		{
			tileMap[board[tilePos.x][tilePos.y + i]]++;
			//posMap[GetCellPosFromTile(board[tilePos.x][tilePos.y + i])]++;
			connectedTiles.push_back(board[tilePos.x][tilePos.y + i]);
			downCount++;
		}

		if (rightCount == 5 || leftCount == 5 || upCount == 5 || downCount == 5)
		{
			qwirkle = true;
		}
	}

	if (qwirkle)
	{
		SetQwirkleText("QWIRKLE");
		qwirkleCount = 6;
	}

	return tileMap.size() + qwirkleCount;
}

/// <summary>
/// Sets the text to what ever you put in the paremters
/// </summary>
/// <param name="text">String for the text</param>
void Game::SetQwirkleText(std::string text)
{
	qwirkleClock.restart();

	qwirkleString = text;
	qwirkleText.setString(qwirkleString);
	qwirkleText.setOrigin(qwirkleText.getLocalBounds().width / 2, qwirkleText.getLocalBounds().height / 2);

	
}

///////////////
///////AI//////
///////////////

/// <summary>
/// The main loop that deals with the AI tile placement
/// </summary>
/// 
void Game::SortAiTurn()
{
	sf::Vector2i move;

	if (isFirstMoveOfGame)
	{
		move = { rand() % Global::ROWS_COLUMNS,rand() % Global::ROWS_COLUMNS };
		sameLineVector = move;
		AiPlaceTileOnBoard(move);
		isFirstMoveOfGame = false;
		
	}
	else
	{
		if (difficulty == EASY)
		{
			if (movesInTurnCount < MAX_AI_MOVES_EASY)
			{
				move = MakeAiMove();
			}
			else
			{
				move = { -1, -1 };
			}	
		}
		if (difficulty == MEDIUM)
		{
			if (movesInTurnCount < MAX_AI_MOVES_MEDIUM)
			{
				move = MakeAiMove();
			}
			else
			{
				move = { -1, -1 };
			}
		}
		if (difficulty == HARD)
		{
			if (movesInTurnCount < MAX_AI_MOVES_HARD)
			{
				move = MakeAiMove();
			}
			else
			{
				move = { -1, -1 };
			}
		}
		

		if (move.x == -1)
		{
			_Output_To_Screen("AI has no valid move: skipping turn");
			NextTurn();
			return;
		}
		else
		{
			AiPlaceTileOnBoard(move);

		}
	}
	movesInTurnCount++;
	aiClock.restart();

}


/// <summary>
/// Gets the valid move from the main logic function
/// </summary>
/// <returns>Valid move position</returns>
sf::Vector2i Game::MakeAiMove()
{
	sf::Vector2i movePos;

	movePos = GetAiMove();
	// If the vector2i map worked, I could sort out whats the best score here and return the best pos to move

	return movePos;
}


/// <summary>
/// Has all the logic that decides where the AI will place the tile
/// </summary>
/// <returns></returns>
sf::Vector2i Game::GetAiMove()
{
	sf::Vector2i movePos = { -1, -1 };
	bool isValid = false;

	std::vector<Tile> neighbourTiles;
	std::vector<sf::Vector2i> validMovePositions;

	allPiecesOnBoard = FindAllValidTilesOnBoard();

	for each (Tile validTile in allPiecesOnBoard)
	{
		sf::Vector2i tileOnBoardPos = GetCellPosFromTile(validTile);
		neighbourTiles = GetAiNeighbours(tileOnBoardPos.x, tileOnBoardPos.y);

		for each (Tile tileToPlace in neighbourTiles)
		{
			if (tileToPlace.GetCurrentColor() == EMPTY_C)
			{
				// GETTING THE CELL POS OF THE TILE YOU WANT TO PLACE
				movePos = GetCellPosFromTile(tileToPlace);

				currentCellPos = movePos;

				isValid = CheckValidTileOrShapeAi(validTile);
				
				if (isValid)
				{
					isValid = CheckFurtherInLine(aiTiles[aiSelectedTile], validTile);
				}

				Tile selectedTile = aiTiles[aiSelectedTile];

				if (isValid && movesInTurnCount != 0)
				{
					if (movePos.x == CheckIfPlacingInSameLine(movePos.x, movePos.y) || movePos.y == CheckIfPlacingInSameLine(movePos.x, movePos.y))
					{
						isValid = true;
					}
					else
					{
						isValid = false;
					}
				}

				if (isValid)
				{
					movePos = GetCellPosFromTile(tileToPlace);
					validMovePositions.push_back(movePos);
					validAiMoves.push_back(movePos);
					validAiTiles.push_back(selectedTile);
					//int x = AddScoresForTurn(validAiTiles);
					// Ideally, I'd make a std::map of Tile and std::vector<sf::vector2i> to store each tile and its possible positions it can move to
					// But i cant get the map to work with sf::vector2i
					// std::map <Tile, std::vector<sf::Vector2i>> tileMap;
					// tileMap.insert(selectedTile, validAiMoves);
					// That way I could compare each tile and all its moves and then check which one would get the best score

					if (movesInTurnCount == 0)
					{
						firstTilePlacedInTurn = board[movePos.x][movePos.y];
						sameLineVector = { movePos.x, movePos.y };
					}

					return movePos;
				}
				else
				{
					movePos = { -1, -1 };
				}
			}
		}
	}
	return movePos;
}

/// <summary>
/// Loops through the tiles on the board and adds the tiles that arent empty to a vector
/// </summary>
/// <returns>Vector of tiles on the board</returns>
std::vector<Tile> Game::FindAllValidTilesOnBoard()
{
	// REMOVE ALL TILES FROM VECTOR AT START
	allPiecesOnBoard.clear();

	std::vector<Tile> tilesOnBoard;
	for (int row = 0; row < Global::ROWS_COLUMNS; row++)
	{
		for (int col = 0; col < Global::ROWS_COLUMNS; col++)
		{
			if (board[row][col].GetCurrentColor() != EMPTY_C)
			{
				// ADDS ALL PIECES ON BOARD TO THE LIST
				tilesOnBoard.push_back(board[row][col]);
			}
		}

	}
	return tilesOnBoard;
}

/// <summary>
/// Given a row and col, find all 4 neighbours of this tile
/// </summary>
/// <param name="row">Gets the row on the board</param>
/// <param name="col">Gets the col on the board</param>
/// <returns></returns>
std::vector<Tile> Game::GetAiNeighbours(int row, int col)
{
	std::vector<Tile> neighbourTiles;

	for (int i = row - 1; i <= row + 1; ++i)
	{
		for (int j = col - 1; j <= col + 1; ++j)
		{
			if ((i == row && j == col) || (i != row && j != col)) { continue; }

			if (i >= 0 && i < Global::ROWS_COLUMNS
				&& j >= 0 && j < Global::ROWS_COLUMNS)
			{
				neighbourTiles.push_back(board[i][j]);
			}
		}
	}
	return neighbourTiles;
}

/// <summary>
/// Checks the neighbours of the tile and sees if its a valid move
/// </summary>
/// <param name="tile"></param>
/// <returns>A bool to see if its a valid move or not</returns>
bool Game::CheckValidTileOrShapeAi(Tile tile)
{
	bool isValid = false;

	for (int i = 0; i < 6; i++)
	{
		if (!aiTiles[i].GetUsed())
		{
			if (aiTiles[i].GetCurrentColor() == tile.GetCurrentColor() ||
				aiTiles[i].GetCurrentShape() == tile.GetCurrentShape())
			{
				aiCurrentPiece = aiTiles[i].GetCurrentPiece();
				aiSelectedTile = i;
				isValid = true;
				break;
			}
		}
	}

	sf::Vector2i temp = GetCellPosFromTile(tile);
	std::vector<Tile> neighbourTiles = GetAiNeighbours(currentCellPos.x, currentCellPos.y);

	for each (Tile tile in neighbourTiles)
	{
		if (tile.GetCurrentColor() != EMPTY_C)
		{
			if (aiTiles[aiSelectedTile].GetCurrentColor() != tile.GetCurrentColor() &&
				aiTiles[aiSelectedTile].GetCurrentShape() != tile.GetCurrentShape())

			{
				isValid = false;
				return isValid;
			}
		}
	}

	return isValid;
}

/// <summary>
/// This is what places the valid tile on the board
/// </summary>
/// <param name="boardPos">Takes the position on the board</param>
void Game::AiPlaceTileOnBoard(sf::Vector2i boardPos)
{
	currentCellPos = boardPos;
	int row = boardPos.x;
	int col = boardPos.y;

	if (isFirstMoveOfGame)
	{
		aiCurrentPiece = aiTiles[0].GetCurrentPiece();
		board[row][col].SetPiece(aiCurrentPiece);
		firstTilePlacedInTurn = board[row][col];
	}
	else if (!aiTiles[aiSelectedTile].GetUsed())
	{
		board[row][col].SetPiece(aiCurrentPiece);
	}
	std::cout << "AI has placed a " + board[row][col].tileName + " on tile " + std::to_string(row) + ", " + std::to_string(col) + "\n";
	tilesPlacedInTurnForScore.push_back(board[row][col]);
	board[row][col].tile.setFillColor(sf::Color::White);
	board[row][col].tile.setTexture(&tileTexture);
	aiTiles[aiSelectedTile].SetUsed();

}

//int MiniMax(int n)
//{
//	int score = 0;
//
//	if N is deep enough then
//		return the estimated score of this leaf
//	else
//		Let N1, N2, .., Nm be the successors of N;
//	if N is a Min node then
//		return min{ MINIMAX(N1), .., MINIMAX(Nm) }
//	else
//		return max{ MINIMAX(N1), .., MINIMAX(Nm) }
//	return score;
//}

//int Game::MiniMax(int depth, bool isMaximizingPlayer)
//{
//	// Base case: check if the game has reached a terminal state
//	// and return the evaluation score for that state
//
//	// Pseudo code: Check if the game is in a terminal state
//	// if (depth == 0 or game is over)
//	//     return EvaluateGameState();
//
//	// Check if it's the maximizing player's turn
//	if (isMaximizingPlayer)
//	{
//		int maxEval = std::numeric_limits<int>::min();
//
//		// Iterate over all possible moves (child nodes)
//		for each (possible move for maximizing player)
//		{
//			int eval = MiniMax(depth - 1, false);  // Recur for the next level (opponent's turn)
//			maxEval = std::max(maxEval, eval);
//		}
//
//		return maxEval;
//	}
//	else  // Minimizing player's turn
//	{
//		int minEval = std::numeric_limits<int>::max();
//
//		// Iterate over all possible moves (child nodes)
//		for each (possible move for minimizing player)
//		{
//			int eval = MiniMax(depth - 1, true);  // Recur for the next level (maximizing player's turn)
//			minEval = std::min(minEval, eval);
//		}
//
//		return minEval;
//	}
//}
