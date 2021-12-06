#pragma once

#include "Snake.h"
#include "Food.h"

//inherits from base snake class
class AISnake : public Snake {
private:
	std::vector<Food> m_allFoodPositions;
	std::vector<sf::Vector2f> m_allSnakePositions;
	std::vector<sf::Vector2f> m_nextMoves;
	std::vector<EDirection> m_nextMoveDirections;
	std::vector<float> m_nextMoveDistances;
	std::vector<float> m_foodDistances;
	sf::Vector2f m_closestFoodPosition;
	float m_minFoodDistance;
	float m_shortestDistance;
public:
	AISnake(sf::Vector2f position, int gridSize, sf::Color colour, EDirection startDirection, int windowWidth, std::string name);
	void update() override;
	void getFoodPositions(std::vector<Food> allFood);
	void getAllSnakePositions(std::vector<sf::Vector2f> allSnakes);
	bool getFutureSnakeCollisions(EDirection newDirection);
	void findNextSnakeMoves();
	void findBestNextMove();
	void getAllFoodDistances();
	void getClostestFood();
	int getMinFoodDistance();
	sf::Vector2f getClosestFoodPosition();
};
