#include "AISnake.h"

AISnake::AISnake(sf::Vector2f position, int gridSize, sf::Color colour, EDirection startDirection, int windowWidth, std::string name) : 
	Snake(position, gridSize, colour, startDirection, windowWidth, name),
	m_minFoodDistance(0),
	m_shortestDistance(0)
{
}

void AISnake::update()
{
	bool foodVisible = false;
	//check if any food is visible
	for (auto& food : m_allFoodPositions) {
		if (food.getVisible()) {
			foodVisible = true;
			break;
		}
	}


	if (foodVisible){
		//if food is visible, move towards it

		if (!m_segments.empty()) {
			//get closest food to move towards to
			getAllFoodDistances();
			getClostestFood();
		}
		
		//get next possible moves
		findNextSnakeMoves();

		//sets direction to the next best position towards the food
		findBestNextMove();

		move();
	}
	else {
		//if no food is visible, move in a random direction

		bool movePossible = false;

		//directions checked
		bool upChecked = false;
		bool rightChecked = false;
		bool downChecked = false;
		bool leftChecked = false;


		//Move snake randomly in a safe direction
		if (!m_segments.empty()) {

			while ((!movePossible) && m_alive) {
				//get random new direction
				EDirection newDirection = EDirection::eRight;

				switch (rand() % 4) {
				case 0:
					newDirection = EDirection::eUp;
					break;
				case 1:
					newDirection = EDirection::eRight;
					break;
				case 2:
					newDirection = EDirection::eDown;
					break;
				case 3:
					newDirection = EDirection::eLeft;
					break;
				}

				//check if up is possible
				if (newDirection == EDirection::eUp && m_direction != EDirection::eDown && !getFutureSnakeCollisions(newDirection))
				{
					m_direction = EDirection::eUp;
					movePossible = true;
				}
				//check if up is not possible
				else if (newDirection == EDirection::eUp && (m_direction == EDirection::eDown || getFutureSnakeCollisions(newDirection)))
				{
					upChecked = true;
				}
				//check if right is possible
				else if (newDirection == EDirection::eRight && m_direction != EDirection::eLeft && !getFutureSnakeCollisions(newDirection))
				{
					m_direction = EDirection::eRight;
					movePossible = true;
				}
				//check if right is not possible
				else if (newDirection == EDirection::eRight && (m_direction == EDirection::eLeft || getFutureSnakeCollisions(newDirection)))
				{
					rightChecked = true;
				}
				//check if down is possible
				else if (newDirection == EDirection::eDown && m_direction != EDirection::eUp && !getFutureSnakeCollisions(newDirection))
				{
					m_direction = EDirection::eDown;
					movePossible = true;
				}
				//check if down is not possible
				else if (newDirection == EDirection::eDown && (m_direction == EDirection::eUp || getFutureSnakeCollisions(newDirection)))
				{
					downChecked = true;
				}
				//check if left is possible
				else if (newDirection == EDirection::eLeft && m_direction != EDirection::eRight && !getFutureSnakeCollisions(newDirection))
				{
					m_direction = EDirection::eLeft;
					movePossible = true;
				}
				//check if left is not possible
				else if (newDirection == EDirection::eLeft && (m_direction == EDirection::eRight || getFutureSnakeCollisions(newDirection)))
				{
					leftChecked = true;
				}
				else {
					movePossible = false;
				}

				if ((upChecked && rightChecked) && (downChecked && leftChecked)) {
					//if all directions check and no safe move is found, kill the snake
					m_alive = false;
				}
			}

			if (!m_segments.empty()) {
				move();
			}
		}
	}
}

void AISnake::getFoodPositions(std::vector<Food> allFood) {
	//get positions of all food
	m_allFoodPositions.clear();
	for (auto& food : allFood) {
		m_allFoodPositions.push_back(food);
	}
}

void AISnake::getAllSnakePositions(std::vector<sf::Vector2f> allSnakes) {
	//get positions of all snake segment positions
	m_allSnakePositions.clear();
	for (auto& snake : allSnakes) {
		m_allSnakePositions.push_back(snake);
	}
}

bool AISnake::getFutureSnakeCollisions(EDirection newDirection) {
	//check snake for hitting any snake segment, or walls in the next move
	int counter = 0;

	for (auto& segment : m_allSnakePositions) {
		if (!m_segments.empty()) {
			switch (newDirection) {
			case EDirection::eUp:
				if ((!m_batteryActive) && (m_segments.front().y - m_segmentSize) == segment.y && m_segments.front().x == segment.x) {
					counter++;
				}
				else if ((m_segments.front().y - m_segmentSize) < 0) {
					counter++;
				}
				break;
			case EDirection::eRight:
				if ((!m_batteryActive) && (m_segments.front().x + m_segmentSize) == segment.x && m_segments.front().y == segment.y) {
					counter++;
				}
				else if ((m_segments.front().x + m_segmentSize) >= m_windowWidth) {
					counter++;
				}
				break;
			case EDirection::eDown:
				if ((!m_batteryActive) && (m_segments.front().y + m_segmentSize) == segment.y && m_segments.front().x == segment.x) {
					counter++;
				}
				else if ((m_segments.front().y + m_segmentSize) >= m_windowWidth) {
					counter++;
				}
				break;
			case EDirection::eLeft:
				if ((!m_batteryActive) && (m_segments.front().x - m_segmentSize) == segment.x && m_segments.front().y == segment.y) {
					counter++;
				}
				else if ((m_segments.front().x - m_segmentSize) < 0) {
					counter++;
				}
				break;
			}

		}
	}
	//return true if next move is not safe for the snake
	if (counter > 0) {
		return true;
	}
	else {
		return false;
	}
}

void AISnake::findNextSnakeMoves() {
	//get the position of the next possible moves, based on the current direction.
	//provided those moves are safe

	m_nextMoves.clear();
	m_nextMoveDirections.clear();

	if (!m_segments.empty()) {
		switch (m_direction) {
		case EDirection::eUp:
			//left move
			if (!getFutureSnakeCollisions(EDirection::eLeft)) {
				m_nextMoves.push_back(sf::Vector2f(m_segments.front().x - m_segmentSize, m_segments.front().y));
				m_nextMoveDirections.push_back(EDirection::eLeft);
			}

			//up move
			if (!getFutureSnakeCollisions(EDirection::eUp)) {
				m_nextMoves.push_back(sf::Vector2f(m_segments.front().x, m_segments.front().y - m_segmentSize));
				m_nextMoveDirections.push_back(EDirection::eUp);
			}

			//right move
			if (!getFutureSnakeCollisions(EDirection::eRight)) {
				m_nextMoves.push_back(sf::Vector2f(m_segments.front().x + m_segmentSize, m_segments.front().y));
				m_nextMoveDirections.push_back(EDirection::eRight);
			}
			break;
		case EDirection::eRight:
			//right move
			if (!getFutureSnakeCollisions(EDirection::eRight)) {
				m_nextMoves.push_back(sf::Vector2f(m_segments.front().x + m_segmentSize, m_segments.front().y));
				m_nextMoveDirections.push_back(EDirection::eRight);
			}

			//down move
			if (!getFutureSnakeCollisions(EDirection::eDown)) {
				m_nextMoves.push_back(sf::Vector2f(m_segments.front().x, m_segments.front().y + m_segmentSize));
				m_nextMoveDirections.push_back(EDirection::eDown);
			}

			//up move
			if (!getFutureSnakeCollisions(EDirection::eUp)) {
				m_nextMoves.push_back(sf::Vector2f(m_segments.front().x, m_segments.front().y - m_segmentSize));
				m_nextMoveDirections.push_back(EDirection::eUp);
			}
			break;
		case EDirection::eDown:
			//left move
			if (!getFutureSnakeCollisions(EDirection::eLeft)) {
				m_nextMoves.push_back(sf::Vector2f(m_segments.front().x - m_segmentSize, m_segments.front().y));
				m_nextMoveDirections.push_back(EDirection::eLeft);
			}

			//down move
			if (!getFutureSnakeCollisions(EDirection::eDown)) {
				m_nextMoves.push_back(sf::Vector2f(m_segments.front().x, m_segments.front().y + m_segmentSize));
				m_nextMoveDirections.push_back(EDirection::eDown);
			}

			//right move
			if (!getFutureSnakeCollisions(EDirection::eRight)) {
				m_nextMoves.push_back(sf::Vector2f(m_segments.front().x + m_segmentSize, m_segments.front().y));
				m_nextMoveDirections.push_back(EDirection::eRight);
			}
			break;
		case EDirection::eLeft:
			//up move
			if (!getFutureSnakeCollisions(EDirection::eUp)) {
				m_nextMoves.push_back(sf::Vector2f(m_segments.front().x, m_segments.front().y - m_segmentSize));
				m_nextMoveDirections.push_back(EDirection::eUp);
			}

			//down move
			if (!getFutureSnakeCollisions(EDirection::eDown)) {
				m_nextMoves.push_back(sf::Vector2f(m_segments.front().x, m_segments.front().y + m_segmentSize));
				m_nextMoveDirections.push_back(EDirection::eDown);
			}

			//left move
			if (!getFutureSnakeCollisions(EDirection::eLeft)) {
				m_nextMoves.push_back(sf::Vector2f(m_segments.front().x - m_segmentSize, m_segments.front().y));
				m_nextMoveDirections.push_back(EDirection::eLeft);
			}
			break;
		}
	}
}

void AISnake::findBestNextMove() {
	//find the move that takes the snake closest to the food

	m_nextMoveDistances.clear();

	for (auto& nextMove : m_nextMoves) {
		//get distance of all possible moves
		int xDistance = abs((nextMove.x - getClosestFoodPosition().x));
		int yDistance = abs((nextMove.y - getClosestFoodPosition().y));

		m_nextMoveDistances.push_back(sqrt((pow(xDistance, 2) + pow(yDistance, 2))));
	}

	if (!m_nextMoveDistances.empty()) {
		float currentSmallest = m_nextMoveDistances[0];
		int currentSmallestIndex = 0;
		//find the corresponding direction for the best move
		for (size_t i = 0; i < m_nextMoveDistances.size(); i++) {
			if (m_nextMoveDistances[i] < currentSmallest) {
				currentSmallest = m_nextMoveDistances[i];
				currentSmallestIndex = i;
			}
		}

		m_direction = m_nextMoveDirections[currentSmallestIndex];

	}

}

void AISnake::getAllFoodDistances() {
	//get the distances from the snake head to all food, to determin the closest food to the snake

	m_foodDistances.clear();

	sf::Vector2f m_startPosition = m_segments.front();
	sf::Vector2f m_endPosition;

	for (auto& food : m_allFoodPositions) {
		if (food.getVisible()) {
			m_endPosition.x = food.getPosition().x;
			m_endPosition.y = food.getPosition().y;

			int xDistance = abs((m_startPosition.x - m_endPosition.x));
			int yDistance = abs((m_startPosition.y - m_endPosition.y));

			m_minFoodDistance = sqrt((pow(xDistance, 2) + pow(yDistance, 2)));
			
			m_foodDistances.push_back(m_minFoodDistance);
		}
	}

}

void AISnake::getClostestFood() {
	//find the shortest distance
	std::sort(m_foodDistances.begin(), m_foodDistances.end());
	m_shortestDistance = m_foodDistances[0];

	sf::Vector2f m_startPosition = m_segments.front();
	sf::Vector2f m_endPosition;

	for (auto& food : m_allFoodPositions) {
		m_endPosition.x = food.getPosition().x;
		m_endPosition.y = food.getPosition().y;

		int xDistance = abs((m_startPosition.x - m_endPosition.x));
		int yDistance = abs((m_startPosition.y - m_endPosition.y));

		m_minFoodDistance = sqrt((pow(xDistance, 2) + pow(yDistance, 2)));

		//find the food position the corresponds to this shortest distance
		if (m_minFoodDistance == m_shortestDistance) {
			m_closestFoodPosition.x = food.getPosition().x;
			m_closestFoodPosition.y = food.getPosition().y;
			break;
		}

	}

}

int AISnake::getMinFoodDistance() {
	return m_minFoodDistance;
}

sf::Vector2f AISnake::getClosestFoodPosition() {
	return m_closestFoodPosition;
}