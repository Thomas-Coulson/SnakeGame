#include "Food.h"

Food::Food(int squareSize, int gameWidth, int gameHeight) :
	m_colour(sf::Color::Red),
	m_type(EFoodType::eSmall),
	m_squareSize(squareSize),
	m_visible(false),
	m_growSize(0)
{
	//randomise initial spawn position
	m_position.x = rand() % ((gameWidth / m_squareSize) - 1) * m_squareSize;
	m_position.y = rand() % ((gameHeight / m_squareSize) - 1) * m_squareSize;

	//randomise food type
	switch (rand() % 3) {
	case 0:
		m_type = EFoodType::eSmall;
		m_growSize = (int)m_type;
		m_colour = sf::Color(255, 130, 236, 255);
		break;
	case 1:
		m_type = EFoodType::eMedium;
		m_growSize = (int)m_type;
		m_colour = sf::Color(255, 64, 64, 255);
		break;
	case 2:
		m_type = EFoodType::eLarge;
		m_growSize = (int)m_type;
		m_colour = sf::Color(182, 71, 255, 255);
		break;
	default:
		break;
	}
}

void Food::makeVisible(int gameWidth, int gameHeight, std::vector<sf::Vector2f>& allPositions) {
	if (!m_visible) {
		if (rand() % 50 == 1) {
		//give the food a random chance to spawn

			//randomise new spawn position, as long as new position doesnt ovlap with anything
			while (std::find(allPositions.begin(), allPositions.end(), m_position) != allPositions.end()) {
				m_position.x = rand() % ((gameWidth / m_squareSize) - 1) * m_squareSize;
				m_position.y = rand() % ((gameHeight / m_squareSize) - 1) * m_squareSize;
			}

			//randomise food type
			switch (rand() % 3) {
			case 0:
				m_type = EFoodType::eSmall;
				m_growSize = (int)m_type;
				m_colour = sf::Color(255, 130, 236, 255);
				break;
			case 1:
				m_type = EFoodType::eMedium;
				m_growSize = (int)m_type;
				m_colour = sf::Color(255, 64, 64, 255);
				break;
			case 2:
				m_type = EFoodType::eLarge;
				m_growSize = (int)m_type;
				m_colour = sf::Color(182, 71, 255, 255);
				break;
			default:
				break;
			}

			//make visible
			m_visible = true;

		}
	}
}

void Food::makeInvisible() {
	m_visible = false;
}

bool Food::getVisible() {
	return m_visible;
}

void Food::render(sf::RenderWindow& window) {
	if (m_visible) {
		sf::RectangleShape food(sf::Vector2f(m_squareSize, m_squareSize));
		food.setFillColor(m_colour);
		food.setPosition(m_position);
		window.draw(food);
	}
}

sf::Vector2f Food::getPosition() {
	return m_position;
}

int Food::getGrowSize() {
	return m_growSize;
}