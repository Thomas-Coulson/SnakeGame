#include "Snake.h"

Snake::Snake(sf::Vector2f position, int gridSize, sf::Color colour, EDirection startDirection, int windowWidth, const std::string& name) :
	m_direction(startDirection),
	m_windowWidth(windowWidth),
	m_segmentSize(gridSize),
	m_startLength(5),
	m_colour(colour),
	m_growAmount(m_startLength - 1),
	m_alive(true),
	m_score(0),
	m_batteryActive(false),
	m_deltaBatteryTime(0),
	m_activeTime(0),
	m_name(name)

{
	sf::Vector2f nextPosition(position.x, position.y);
	m_segments.push_back(nextPosition);
}

//move the snake in the direction in which it is facing
void Snake::move() {

	if (!m_segments.empty()) {

		//if the snake needs to grow, dont pop from the back of the list
		if (m_growAmount > 0) {
			m_growAmount--;
		}
		else {
			m_segments.pop_back();
		}

		switch (m_direction) {
		case EDirection::eUp:
			if (!m_segments.empty()) {
				m_segments.push_front(sf::Vector2f(m_segments.front().x, m_segments.front().y - m_segmentSize));
			}
			break;
		case EDirection::eRight:
			if (!m_segments.empty()) {
				m_segments.push_front(sf::Vector2f(m_segments.front().x + m_segmentSize, m_segments.front().y));
			}
			break;
		case EDirection::eDown:
			if (!m_segments.empty()) {
				m_segments.push_front(sf::Vector2f(m_segments.front().x, m_segments.front().y + m_segmentSize));
			}
			break;
		case EDirection::eLeft:
			if (!m_segments.empty()) {
				m_segments.push_front(sf::Vector2f(m_segments.front().x - m_segmentSize, m_segments.front().y));
			}
			break;
		default:
			break;
		}
	}
}

std::list<sf::Vector2f> Snake::getSegmentPositions() {
	return m_segments;
}

std::list<sf::Vector2f> Snake::getElectricPositions() {
	return m_electricPositions;
}

void Snake::checkFoodCollisions(Food& food, int gameWidth, int gameHeight) {
	//if the snake head overlaps the food,
	if (!m_segments.empty()) {
		if (m_segments.front() == food.getPosition() && food.getVisible()) {
			//make food invisible, grow the snake, and add score
			food.makeInvisible();
			m_growAmount += food.getGrowSize();
			m_score += (food.getGrowSize() * 10);
		}
	}
}

void Snake::checkBatteryCollisions(Battery& battery, int gameWidth, int gameHeight) {
	//if the snake head overlaps the battery,.
	if (!m_segments.empty()) {
		if (m_segments.front() == battery.getPosition() && battery.getVisible()) {
			//make the battery invisible, add score, and electrify the snake
			battery.makeInvisible();
			m_score += (battery.getScore());
			m_batteryActive = true;
			restartBatteryCountdown();
		}
	}
}

bool Snake::checkWallCollisions(int gameWidth, int gameHeight) {
	//check for snake hitting walls
	for (auto& segment : m_segments) {
		if (!m_segments.empty()) {
			if (m_segments.front().x < 0 || m_segments.front().x >(gameWidth - m_segmentSize)) {
				return true;
			}
			else if (m_segments.front().y < 0 || m_segments.front().y >(gameHeight - m_segmentSize)) {
				return true;
			}
		}
	}
	return false;
}

bool Snake::checkSnakeCollisions(std::vector<sf::Vector2f>& allSnakePositions, std::vector<sf::Vector2f>& allElectricPositions) {	
	//check snake for hitting any snake segment
	int counter = 0;
	int electricCounter = 0;
	bool tailCut = false;
	bool inList = true;
	sf::Vector2f overlapPosition;

	for (auto& otherSegments : allSnakePositions) {
		if (!m_segments.empty()) {
			if (m_segments.front() == otherSegments) {
				counter++;
			}
		}
	}

	//check if an electrified snake collides with this snake
	if (!m_batteryActive) {
		for (auto& electricSegments : allElectricPositions) {
			for (auto& snakeSegment : m_segments) {
				if (snakeSegment == electricSegments) {
					//if tail get cut by electricity

					overlapPosition = snakeSegment;
					tailCut = true;

				}
			}
		}

		//if electrified collision occurs, cut this snakes tail
		if (tailCut) {
			while (inList) {
				if (!m_segments.empty()) {
					if (overlapPosition != m_segments.front()) {
						m_score -= 5;
					}

					m_segments.pop_back();

					//if overlapPosition is in m_segments, loop again
					inList = std::find(m_segments.begin(), m_segments.end(), overlapPosition) != m_segments.end();
				}
				else {
					m_alive = false;
				}
			}
		}
	}

	if (m_batteryActive) {
		//if this snake is electrified, only die if it collides with itself
		int overlap = 0;
		for (auto& segment : m_segments) {
			if (m_segments.front() == segment) {
				overlap++;
			}
		}
		if (overlap > 1) {

			return true;
		}
		else {
			return false;
		}
	}
	else if (counter > 1) {
		//snake will die if any snake positions overlap
		return true;
	}
	else {
		return false;
	}
}

void Snake::getBatteryAura() {

	m_electricPositions.clear();

	sf::Vector2f currentPosition;
	sf::Vector2f frontPosition;
	sf::Vector2f backPosition;

	int currentListPosition = 0;
	int nextListPosition = 0;

	bool rightBlocked = false;
	bool downBlocked = false;
	bool leftBlocked = false;
	bool upBlocked = false;

	//first get the current segment location in list
	for (auto& currentSegment : m_segments) {
		//set currentPosition
		currentPosition.x = currentSegment.x;
		currentPosition.y = currentSegment.y;

		for (auto& nextSegment : m_segments) {
			//check behind current Position
			if (nextListPosition == (currentListPosition - 1)) {
				backPosition.x = nextSegment.x;
				backPosition.y = nextSegment.y;

				if ((currentPosition.x - backPosition.x) < 0) {
					//right side is blocked
					rightBlocked = true;
				}
				else if ((currentPosition.x - backPosition.x) > 0) {
					//left side is blocked
					leftBlocked = true;
				}
				else if ((currentPosition.y - backPosition.y) < 0) {
					//down side is blocked
					downBlocked = true;
				}
				else if ((currentPosition.y - backPosition.y) > 0) {
					//up side is blocked
					upBlocked = true;
				}
			}
			//check infront of current position
			else if (nextListPosition == (currentListPosition + 1)) {
				frontPosition.x = nextSegment.x;
				frontPosition.y = nextSegment.y;

				if ((currentPosition.x - frontPosition.x) < 0) {
					//right side is blocked
					rightBlocked = true;
				}
				else if ((currentPosition.x - frontPosition.x) > 0) {
					//left side is blocked
					leftBlocked = true;
				}
				else if ((currentPosition.y - frontPosition.y) < 0) {
					//down side is blocked
					downBlocked = true;
				}
				else if ((currentPosition.y - frontPosition.y) > 0) {
					//up side is blocked
					upBlocked = true;
				}
			}

			nextListPosition++;
		}

		//add non-blocked positions to electric vector
		if (!rightBlocked) {
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (1 * m_segmentSize), currentPosition.y));
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (2 * m_segmentSize), currentPosition.y));
		}
		if (!leftBlocked) {
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (1 * m_segmentSize), currentPosition.y));
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (2 * m_segmentSize), currentPosition.y));
		}
		if (!upBlocked) {
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x, currentPosition.y - (1 * m_segmentSize)));
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x, currentPosition.y - (2 * m_segmentSize)));
		}
		if (!downBlocked) {
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x, currentPosition.y + (1 * m_segmentSize)));
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x, currentPosition.y + (2 * m_segmentSize)));
		}

		if (currentPosition == m_segments.front() || currentPosition == m_segments.back()) {
			if (rightBlocked) {
				//top left
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (1 * m_segmentSize), currentPosition.y - (1 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (2 * m_segmentSize), currentPosition.y - (1 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (1 * m_segmentSize), currentPosition.y - (2 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (2 * m_segmentSize), currentPosition.y - (2 * m_segmentSize)));

				//bottom left
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (1 * m_segmentSize), currentPosition.y + (1 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (2 * m_segmentSize), currentPosition.y + (1 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (1 * m_segmentSize), currentPosition.y + (2 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (2 * m_segmentSize), currentPosition.y + (2 * m_segmentSize)));
			}
			else if (upBlocked) {
				//bottom left
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (1 * m_segmentSize), currentPosition.y + (1 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (2 * m_segmentSize), currentPosition.y + (1 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (1 * m_segmentSize), currentPosition.y + (2 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (2 * m_segmentSize), currentPosition.y + (2 * m_segmentSize)));

				//bottom right
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (1 * m_segmentSize), currentPosition.y + (1 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (2 * m_segmentSize), currentPosition.y + (1 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (1 * m_segmentSize), currentPosition.y + (2 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (2 * m_segmentSize), currentPosition.y + (2 * m_segmentSize)));
			}
			else if (leftBlocked) {
				//top right
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (1 * m_segmentSize), currentPosition.y - (1 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (2 * m_segmentSize), currentPosition.y - (1 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (1 * m_segmentSize), currentPosition.y - (2 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (2 * m_segmentSize), currentPosition.y - (2 * m_segmentSize)));

				//bottom right
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (1 * m_segmentSize), currentPosition.y + (1 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (2 * m_segmentSize), currentPosition.y + (1 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (1 * m_segmentSize), currentPosition.y + (2 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (2 * m_segmentSize), currentPosition.y + (2 * m_segmentSize)));
			}
			else if (downBlocked) {
				//top left
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (1 * m_segmentSize), currentPosition.y - (1 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (2 * m_segmentSize), currentPosition.y - (1 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (1 * m_segmentSize), currentPosition.y - (2 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (2 * m_segmentSize), currentPosition.y - (2 * m_segmentSize)));

				//top right
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (1 * m_segmentSize), currentPosition.y - (1 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (2 * m_segmentSize), currentPosition.y - (1 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (1 * m_segmentSize), currentPosition.y - (2 * m_segmentSize)));
				m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (2 * m_segmentSize), currentPosition.y - (2 * m_segmentSize)));
			}
		}

		if (rightBlocked && downBlocked) {
			//top left
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (1 * m_segmentSize), currentPosition.y - (1 * m_segmentSize)));
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (2 * m_segmentSize), currentPosition.y - (1 * m_segmentSize)));
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (1 * m_segmentSize), currentPosition.y - (2 * m_segmentSize)));
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (2 * m_segmentSize), currentPosition.y - (2 * m_segmentSize)));
		}
		else if (upBlocked && rightBlocked) {
			//bottom left
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (1 * m_segmentSize), currentPosition.y + (1 * m_segmentSize)));
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (2 * m_segmentSize), currentPosition.y + (1 * m_segmentSize)));
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (1 * m_segmentSize), currentPosition.y + (2 * m_segmentSize)));
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x - (2 * m_segmentSize), currentPosition.y + (2 * m_segmentSize)));
		}
		else if (upBlocked && leftBlocked) {
			//bottom right
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (1 * m_segmentSize), currentPosition.y + (1 * m_segmentSize)));
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (2 * m_segmentSize), currentPosition.y + (1 * m_segmentSize)));
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (1 * m_segmentSize), currentPosition.y + (2 * m_segmentSize)));
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (2 * m_segmentSize), currentPosition.y + (2 * m_segmentSize)));
		}
		else if (downBlocked && leftBlocked) {
			//top right
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (1 * m_segmentSize), currentPosition.y - (1 * m_segmentSize)));
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (2 * m_segmentSize), currentPosition.y - (1 * m_segmentSize)));
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (1 * m_segmentSize), currentPosition.y - (2 * m_segmentSize)));
			m_electricPositions.push_back(sf::Vector2f(currentPosition.x + (2 * m_segmentSize), currentPosition.y - (2 * m_segmentSize)));
		}

		rightBlocked = false;
		downBlocked = false;
		leftBlocked = false;
		upBlocked = false;

		currentListPosition++;
		nextListPosition = 0;
	}
}

bool Snake::getAlive() {
	return m_alive;
}

void Snake::setAlive(bool alive) {
	m_alive = alive;
}

void Snake::clearAllSegments() {
	m_segments.clear();
}

sf::Color Snake::getColour() {
	return m_colour;
}

int Snake::getScore() {
	return m_score;
}

bool Snake::getBatteryActive() {
	return m_batteryActive;
}

void Snake::restartBatteryCountdown() {
	//when a battery is picked up, start the timer
	m_batteryTimer.restart();
	//the battery will last between 1 and 3 seconds
	m_activeTime = (rand() % 3000) + 1000;
}

void Snake::checkBatteryCountdown() {
	//remove battery effect after a specific time
	m_deltaBatteryTime = m_batteryTimer.getElapsedTime().asMilliseconds();
	if (m_deltaBatteryTime >= m_activeTime) {
		m_batteryActive = false;
		m_electricPositions.clear();
	}
}

std::string Snake::getName() {
	return m_name;
}

void Snake::render(sf::RenderWindow& window) {
	//draw snake
	for (auto& position : m_segments) {
		sf::RectangleShape snake(sf::Vector2f(m_segmentSize, m_segmentSize));
		snake.setFillColor(m_colour);
		snake.setPosition(position);
		window.draw(snake);
	}

	//draw any electric areas (if needed)
	if (m_batteryActive) {
		for (auto& Eposition : m_electricPositions) {
			sf::RectangleShape electric(sf::Vector2f(m_segmentSize, m_segmentSize));
			electric.setFillColor(sf::Color(255, 255, 0, 50));
			electric.setPosition(Eposition);
			window.draw(electric);
		}
	}
}