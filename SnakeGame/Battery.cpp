#include "Battery.h"

Battery::Battery() :
	m_colour(sf::Color::Yellow),
	m_squareSize(20),
	m_visible(false),
	m_scoreValue(50)
{
}

void Battery::makeVisible(int gameWidth, int gameHeight, std::vector<sf::Vector2f>& allPositions) {
	if (!m_visible) {
		while (std::find(allPositions.begin(), allPositions.end(), m_position) != allPositions.end()) {
			//randomise new spawn position, as long as new position doesnt ovlap with anything
			m_position.x = rand() % ((gameWidth / m_squareSize) - 1) * m_squareSize;
			m_position.y = rand() % ((gameHeight / m_squareSize) - 1) * m_squareSize;
		}

		m_visible = true;
		//spawn battery imediately - should not be random
	}
}

void Battery::makeInvisible() {
	m_visible = false;
}

bool Battery::getVisible() {
	return m_visible;
}

int Battery::getScore() {
	return m_scoreValue;
}

void Battery::render(sf::RenderWindow& window) {
	if (m_visible) {
		sf::RectangleShape battery(sf::Vector2f(m_squareSize, m_squareSize));
		battery.setFillColor(m_colour);
		battery.setPosition(m_position);
		window.draw(battery);
	}
}

sf::Vector2f Battery::getPosition() {
	return m_position;
}