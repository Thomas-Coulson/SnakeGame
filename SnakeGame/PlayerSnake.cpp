#include "PlayerSnake.h"

PlayerSnake::PlayerSnake(sf::Vector2f position, int gridSize, sf::Color colour, EDirection startDirection, int windowWidth, std::string name) : 
	Snake(position, gridSize, colour, startDirection, windowWidth, name)
{
}

void PlayerSnake::update() {

	//move the snake in current direction
	move();

	//take user control input
	//change current direction to the user inputed one
	if (!m_segments.empty()) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && m_direction != EDirection::eDown) {
			m_direction = EDirection::eUp;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && m_direction != EDirection::eLeft) {
			m_direction = EDirection::eRight;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && m_direction != EDirection::eUp) {
			m_direction = EDirection::eDown;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && m_direction != EDirection::eRight) {
			m_direction = EDirection::eLeft;
		}
	}

}
