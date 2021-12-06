#pragma once
#include <SFML/Graphics.hpp>
#include <cstdlib>

class Battery
{
private:
	sf::Vector2f m_position;
	sf::Color m_colour;
	int m_squareSize;
	bool m_visible;
	int m_scoreValue;
public:
	Battery();
	void makeVisible(int gameWidth, int gameHeight, std::vector<sf::Vector2f>& allPositions);
	void makeInvisible();
	bool getVisible();
	int getScore();
	void render(sf::RenderWindow& window);
	sf::Vector2f getPosition();
};

