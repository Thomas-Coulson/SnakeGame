#pragma once
#include <list>
#include <SFML/Graphics.hpp>
#include <string>
#include "Food.h"
#include "Battery.h"

enum class EDirection {
	eUp,
	eRight,
	eLeft,
	eDown
};

//base snake class
class Snake
{
protected:
	std::list<sf::Vector2f> m_segments;
	std::list<sf::Vector2f> m_electricPositions;
	EDirection m_direction;
	std::string m_name;
	int m_windowWidth;
	int m_segmentSize;
	int m_startLength;
	sf::Color m_colour;
	int m_growAmount;
	bool m_alive;
	int m_score;
	bool m_batteryActive;
	sf::Clock m_batteryTimer;
	float m_deltaBatteryTime;
	float m_activeTime;
	void move();
	void restartBatteryCountdown();
public:
	Snake(sf::Vector2f position, int gridSize, sf::Color colour, EDirection startDirection, int windowWidth, const std::string& name);
	std::list<sf::Vector2f> getSegmentPositions();
	std::list<sf::Vector2f> getElectricPositions();
	virtual void update() = 0;
	void checkFoodCollisions(Food& food, int gameWidth, int gameHeight);
	void checkBatteryCollisions(Battery& battery, int gameWidth, int gameHeight);
	bool checkWallCollisions(int gameWidth, int gameHeight);
	bool checkSnakeCollisions(std::vector<sf::Vector2f>& allSnakePositions, std::vector<sf::Vector2f>& allElectricPositions);
	void getBatteryAura();
	bool getAlive();
	void setAlive(bool alive);
	void clearAllSegments();
	sf::Color getColour();
	int getScore();
	bool getBatteryActive();
	std::string getName();
	void checkBatteryCountdown();
	void render(sf::RenderWindow& window);
};

