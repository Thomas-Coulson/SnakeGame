#pragma once
#include <SFML/Graphics.hpp>
#include <cstdlib>

//I have 3 different sizes of food, which are worth different score and growth value
enum class EFoodType {
	eSmall = 1,
	eMedium = 2,
	eLarge = 3
};

class Food
{
private:
	sf::Vector2f m_position;
	sf::Color m_colour;
	EFoodType m_type;
	int m_squareSize;
	bool m_visible;
	int m_growSize;
public:
	Food(int squareSize, int gameWidth, int gameHeight);
	void makeVisible(int gameWidth, int gameHeight, std::vector<sf::Vector2f>& allPositions);
	void makeInvisible();
	bool getVisible();
	void render(sf::RenderWindow& window);
	sf::Vector2f getPosition();
	int getGrowSize();
};

