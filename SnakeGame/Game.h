#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <string>
#include "PlayerSnake.h"
#include "AISnake.h"
#include "Food.h"
#include "Battery.h"

class Game
{
private:
	int m_windowWidth;
	int m_windowHeight;
	int m_startingLength;
	int m_gridSize;
	int m_leftMidPoint;
	int m_maxFood;
	int m_snakesAlive;
	bool m_gameOver;
	bool m_menuActive;
	sf::Clock m_gameTime;
	int m_deltaGameTime;
	int m_maxTime;
	sf::Font m_font;

	std::vector<PlayerSnake> m_allPlayerSnakes;
	std::vector<AISnake> m_allAISnakes;
	std::vector<Food> m_allFood;
	std::vector<sf::Vector2f> m_allPositions;
	std::vector<sf::Vector2f> m_allSnakeSegments;
	std::vector<sf::Vector2f> m_allElectricPositions;
	Battery m_battery;
public:
	Game(int windowWidth, int windowHeight, int startingLength, int gridSize, int leftMidPoint);
	void makeGameObjects();
	void update();
	void getAllPositions();
	void getAllSegmentPositions();
	void getAllElectricPositions();
	void checkDeadSnakes();
	void restartGameTime();
	void checkGameOver();
	bool getGameOver();
	void makeMainMenu(sf::RenderWindow& window);
	void makeWinScreen(sf::RenderWindow& window);
	void makeUI(sf::RenderWindow& window);
	void render(sf::RenderWindow& window);
};

