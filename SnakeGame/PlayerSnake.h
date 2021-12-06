#pragma once
#include <list>
#include <SFML/Graphics.hpp>
#include "Snake.h"
#include "Food.h"
#include "Battery.h"

//inherits from base snake class
class PlayerSnake : public Snake {
public:
	PlayerSnake(sf::Vector2f position, int gridSize, sf::Color colour, EDirection startDirection, int windowWidth, std::string name);
	void update() override;
};

