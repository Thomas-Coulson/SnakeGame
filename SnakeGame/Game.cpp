#include "Game.h"

Game::Game(int windowWidth, int windowHeight, int startingLength, int gridSize, int leftMidPoint) :
	//set values for game
	m_windowWidth(windowWidth),
	m_windowHeight(windowHeight),
	m_startingLength(startingLength),
	m_gridSize(gridSize),
	m_leftMidPoint(leftMidPoint),
	m_maxFood(5),
	m_maxTime(90),
	m_gameOver(false),
	m_menuActive(true),
	m_snakesAlive(0),
	m_deltaGameTime(0)
{
}

void Game::makeGameObjects() {
	//make player snake
	//middle left
	PlayerSnake newSnake(sf::Vector2f(((m_startingLength) * m_gridSize), m_leftMidPoint), m_gridSize, sf::Color(18, 199, 57, 255), EDirection::eRight, m_windowWidth, "Player");
	m_allPlayerSnakes.push_back(newSnake);

	//make AI Snakes
	//top left
	AISnake newAISnake1(sf::Vector2f(((m_startingLength) * m_gridSize), 100), m_gridSize, sf::Color::Cyan, EDirection::eRight, m_windowWidth, "Snake 1");
	m_allAISnakes.push_back(newAISnake1);
	//bottom left
	AISnake newAISnake2(sf::Vector2f(((m_startingLength) * m_gridSize), 580), m_gridSize, sf::Color(247, 157, 22, 255), EDirection::eRight, m_windowWidth, "Snake2");
	m_allAISnakes.push_back(newAISnake2);
	//middle right
	AISnake newAISnake3(sf::Vector2f(m_windowWidth - (m_startingLength * m_gridSize), m_leftMidPoint), m_gridSize, sf::Color::Magenta, EDirection::eLeft, m_windowWidth, "Snake3");
	m_allAISnakes.push_back(newAISnake3);
	//top right
	AISnake newAISnake4(sf::Vector2f(m_windowWidth - (m_startingLength*m_gridSize), 100), m_gridSize, sf::Color(0, 136, 255, 255), EDirection::eLeft, m_windowWidth, "Snake4");
	m_allAISnakes.push_back(newAISnake4);
	//bottom right
	AISnake newAISnake5(sf::Vector2f(m_windowWidth - (m_startingLength*m_gridSize), 580), m_gridSize, sf::Color(168, 240, 14, 255), EDirection::eLeft, m_windowWidth, "Snake5");
	m_allAISnakes.push_back(newAISnake5);

	//make food
	for (int i = 0; i < m_maxFood; i++) {
		getAllPositions();
		Food newFood(m_gridSize, m_windowWidth, m_windowHeight);
		m_allFood.push_back(newFood);
	}

	//make battery
	m_battery.makeVisible(m_windowWidth, m_windowHeight, m_allPositions);

}

void Game::update() {
	//display main menu first
	if (m_menuActive && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		m_menuActive = false;
		//only continue if ENTER is pressed on main menu
	}
	else if(!m_menuActive) {
		if (!m_gameOver) {
			//makes a game timer for the 90 second countdown
			m_deltaGameTime = m_gameTime.getElapsedTime().asSeconds();

			//update player snakes
			for (auto& snake : m_allPlayerSnakes) {
				if (snake.getAlive()) {
					snake.update();
					if (snake.getBatteryActive()) {
						snake.getBatteryAura();
						snake.checkBatteryCountdown();
						if (!snake.getBatteryActive() && !m_battery.getVisible()) {
							m_battery.makeVisible(m_windowWidth, m_windowHeight, m_allPositions);
						}
					}
				}
			}

			//update ai snakes
			for (auto& snake : m_allAISnakes) {
				if (snake.getAlive()) {
					snake.getFoodPositions(m_allFood);
					snake.getAllSnakePositions(m_allSnakeSegments);
					snake.update();
					if (snake.getBatteryActive()) {
						snake.getBatteryAura();
						snake.checkBatteryCountdown();
						if (!snake.getBatteryActive() && !m_battery.getVisible()) {
							m_battery.makeVisible(m_windowWidth, m_windowHeight, m_allPositions);
						}
					}
				}
			}

			//get all object positions
			getAllPositions();
			getAllSegmentPositions();

			//spawn food
			for (auto& food : m_allFood) {
				food.makeVisible(m_windowWidth, m_windowHeight, m_allPositions);
			}

			//check if snake eats food
			for (auto& snake : m_allPlayerSnakes) {
				for (auto& food : m_allFood) {
					snake.checkFoodCollisions(food, m_windowWidth, m_windowHeight);
				}
			}

			//check if ai snake eats food
			for (auto& snake : m_allAISnakes) {
				for (auto& food : m_allFood) {
					snake.checkFoodCollisions(food, m_windowWidth, m_windowHeight);
				}
			}

			//check if snake eats battery
			for (auto& snake : m_allPlayerSnakes) {
				snake.checkBatteryCollisions(m_battery, m_windowWidth, m_windowHeight);
			}

			//check if ai snake eats battery
			for (auto& snake : m_allAISnakes) {
				snake.checkBatteryCollisions(m_battery, m_windowWidth, m_windowHeight);
			}

			getAllElectricPositions();
		}
	}
}

void Game::getAllPositions() {

	m_allPositions.clear();

	//get all snake segment positions
	for (auto& snake : m_allPlayerSnakes) {
		for (auto& segment : snake.getSegmentPositions()) {
			m_allPositions.push_back(segment);
		}
		if (snake.getBatteryActive()) {
			for (auto& position : snake.getElectricPositions()) {
				m_allPositions.push_back(position);
			}
		}
	}

	//get all ai snake segment positions
	for (auto& snake : m_allAISnakes) {
		for (auto& segment : snake.getSegmentPositions()) {
			m_allPositions.push_back(segment);
		}
		if (snake.getBatteryActive()) {
			for (auto& position : snake.getElectricPositions()) {
				m_allPositions.push_back(position);
			}
		}
	}

	//get all food positions
	for (auto& food : m_allFood) {
		m_allPositions.push_back(food.getPosition());
	}

	m_allPositions.push_back(m_battery.getPosition());
}

void Game::getAllSegmentPositions() {
	m_allSnakeSegments.clear();

	//get all snake segment positions
	for (auto& snake : m_allPlayerSnakes) {
		for (auto& segment : snake.getSegmentPositions()) {
			m_allSnakeSegments.push_back(segment);
		}
		if (snake.getBatteryActive()) {
			for (auto& position : snake.getElectricPositions()) {
				m_allSnakeSegments.push_back(position);
			}
		}
	}

	//get all ai snake segment positions
	for (auto& snake : m_allAISnakes) {
		for (auto& segment : snake.getSegmentPositions()) {
			m_allSnakeSegments.push_back(segment);
		}
		if (snake.getBatteryActive()) {
			for (auto& position : snake.getElectricPositions()) {
				m_allSnakeSegments.push_back(position);
			}
		}
	}
}

void Game::getAllElectricPositions() {
	m_allElectricPositions.clear();

	//get all electric snake segment positions
	for (auto& snake : m_allPlayerSnakes) {
		for (auto& electric : snake.getElectricPositions()) {
			m_allElectricPositions.push_back(electric);
		}
	}

	//get all electric AI snake segment positions
	for (auto& snake : m_allAISnakes) {
		for (auto& electric : snake.getElectricPositions()) {
			m_allElectricPositions.push_back(electric);
		}
	}
	
}

void Game::checkDeadSnakes() {
	//check if player snakes hit the walls or another snake, and kill them if they do
	for (auto& snake : m_allPlayerSnakes) {
		if (snake.checkWallCollisions(m_windowWidth, m_windowHeight) || snake.checkSnakeCollisions(m_allSnakeSegments, m_allElectricPositions)) {
			snake.setAlive(false);
			snake.clearAllSegments();
		}
	}
	//check if ai snakes hit the walls or another snake, and kill them if they do
	for (auto& snake : m_allAISnakes) {
		if (snake.checkWallCollisions(m_windowWidth, m_windowHeight) || snake.checkSnakeCollisions(m_allSnakeSegments, m_allElectricPositions)) {
			snake.setAlive(false);
			snake.clearAllSegments();
		}
	}
}

void Game::checkGameOver() {

	if (!m_gameOver) {
		m_snakesAlive = 0;
		
		//check how many player snakes are dead
		for (auto& pSnake : m_allPlayerSnakes) {
			if (pSnake.getAlive()) {
				m_snakesAlive++;
			}
		}
		//check how many ai snakes are dead
		for (auto& aSnake : m_allAISnakes) {
			if (aSnake.getAlive()) {
				m_snakesAlive++;
			}
		}

		// if there are less than 2 snakes alive, the game is over
		if (m_snakesAlive < 2) {
			m_gameOver = true;
		}
		// if the time runs out, the game is over
		else if (m_deltaGameTime >= m_maxTime) {
			m_gameOver = true;
		}
	}
}

bool Game::getGameOver() {
	return m_gameOver;
}

void Game::restartGameTime() {
	m_gameTime.restart();
}

void Game::makeMainMenu(sf::RenderWindow& window) {
	//make the main title menu

	//set base font
	if (m_font.loadFromFile("Fonts//AmazDooMLeft.ttf")) {
		m_font.loadFromFile("Fonts//AmazDooMLeft.ttf");
	}

	//make title text
	sf::Text titleText;
	titleText.setFont(m_font);
	titleText.setCharacterSize(200);
	titleText.setFillColor(sf::Color::White);
	titleText.setPosition(sf::Vector2f(375, 50));
	titleText.setString("SNAKE 2");
	window.draw(titleText);

	//set base font
	if (m_font.loadFromFile("Fonts//upheavtt.ttf")) {
		m_font.loadFromFile("Fonts//upheavtt.ttf");
	}

	//make subtitle text
	sf::Text subtitleText;
	subtitleText.setFont(m_font);
	subtitleText.setCharacterSize(60);
	subtitleText.setFillColor(sf::Color::Yellow);
	subtitleText.setPosition(sf::Vector2f(295, 225));
	subtitleText.setString("Electric Boogaloo");
	window.draw(subtitleText);

	//make controls text
	sf::Text controlsText;
	controlsText.setFont(m_font);
	controlsText.setCharacterSize(50);
	controlsText.setFillColor(sf::Color::White);
	controlsText.setPosition(sf::Vector2f(275, 450));
	controlsText.setString("Use Arrow keys to move");
	window.draw(controlsText);

	//make snake colour text
	sf::Text snakeColourText;
	snakeColourText.setFont(m_font);
	snakeColourText.setCharacterSize(50);
	snakeColourText.setFillColor(sf::Color(18, 199, 57, 255));
	snakeColourText.setPosition(sf::Vector2f(240, 550));
	snakeColourText.setString("Your snake is this colour");
	window.draw(snakeColourText);

	//make start game text
	sf::Text startText;
	startText.setFont(m_font);
	startText.setCharacterSize(60);
	startText.setFillColor(sf::Color::White);
	startText.setPosition(sf::Vector2f(150, 700));
	startText.setString("--- press Enter to start ---");
	window.draw(startText);
}

void Game::makeWinScreen(sf::RenderWindow& window) {
	//make the game over win screen

	//set base font
	if (m_font.loadFromFile("Fonts//upheavtt.ttf")) {
		m_font.loadFromFile("Fonts//upheavtt.ttf");
	}

	if (m_snakesAlive == 1) {
		//determins the last player standing
		for (auto& snake : m_allPlayerSnakes) {
			if (snake.getAlive()) {
				sf::Text playerWinText;
				playerWinText.setFont(m_font);
				playerWinText.setCharacterSize(70);
				playerWinText.setFillColor(snake.getColour());
				playerWinText.setPosition(sf::Vector2f(100, 100));
				playerWinText.setString(snake.getName() + " Wins\nAs the last\nsnake standing!");
				window.draw(playerWinText);
			}
		}

		for (auto& snake : m_allAISnakes) {
			if (snake.getAlive()) {
				sf::Text aiWinText;
				aiWinText.setFont(m_font);
				aiWinText.setCharacterSize(70);
				aiWinText.setFillColor(snake.getColour());
				aiWinText.setPosition(sf::Vector2f(100, 100));
				aiWinText.setString(snake.getName() + " Wins\nAs the last\nsnake standing!");
				window.draw(aiWinText);
			}
		}
	}
	else if (m_snakesAlive == 0 || m_deltaGameTime >= m_maxTime) {
		//determins the snake with the highest score

		//go through all snakes, get highest score, show snake with highest score
		int bestScore = 0;
		int pHighScore = 0;
		int pHighScoreIndex = 0;
		int aHighScore = 0;
		int aHighScoreIndex = 0;


		for (size_t i = 0; i < m_allPlayerSnakes.size(); i++) {
			if (m_allPlayerSnakes[i].getScore() > pHighScore) {
				pHighScore = m_allPlayerSnakes[i].getScore();
				pHighScoreIndex = i;
			}
		}

		for (size_t j = 0; j < m_allAISnakes.size(); j++) {
			if (m_allAISnakes[j].getScore() > aHighScore) {
				aHighScore = m_allAISnakes[j].getScore();
				aHighScoreIndex = j;
			}
		}

		if (pHighScore >= aHighScore) {
			//make player Score Win text
			sf::Text playerScoreWinText;
			playerScoreWinText.setFont(m_font);
			playerScoreWinText.setCharacterSize(70);
			playerScoreWinText.setFillColor(m_allPlayerSnakes[pHighScoreIndex].getColour());
			playerScoreWinText.setPosition(sf::Vector2f(100, 100));
			playerScoreWinText.setString(m_allPlayerSnakes[pHighScoreIndex].getName() + " Wins\nBy Score!");
			window.draw(playerScoreWinText);
		}
		else if (aHighScore > pHighScore) {
			//make ai Score Win text
			sf::Text aiScoreWinText;
			aiScoreWinText.setFont(m_font);
			aiScoreWinText.setCharacterSize(70);
			aiScoreWinText.setFillColor(m_allAISnakes[aHighScoreIndex].getColour());
			aiScoreWinText.setPosition(sf::Vector2f(100, 100));
			aiScoreWinText.setString(m_allAISnakes[aHighScoreIndex].getName() + " Wins\nBy Score!");
			window.draw(aiScoreWinText);
		}
	}

	//make play again text
	sf::Text playAgainText;
	playAgainText.setFont(m_font);
	playAgainText.setCharacterSize(40);
	playAgainText.setFillColor(sf::Color::White);
	playAgainText.setPosition(sf::Vector2f(100, 700));
	playAgainText.setString("--- Space to play again ---");
	window.draw(playAgainText);

}

void Game::makeUI(sf::RenderWindow& window) {
	//make dividing line
	sf::RectangleShape line(sf::Vector2f(m_windowHeight, 2));
	line.setPosition(sf::Vector2f(m_windowHeight, 0));
	line.rotate(90);
	window.draw(line);

	//make score text
	//set base font
	if (m_font.loadFromFile("Fonts//upheavtt.ttf")) {
		m_font.loadFromFile("Fonts//upheavtt.ttf");
	}

	//make score title text
	sf::Text titleScoreText;
	titleScoreText.setFont(m_font);
	titleScoreText.setCharacterSize(70);
	titleScoreText.setFillColor(sf::Color::White);
	titleScoreText.setPosition(sf::Vector2f(m_windowHeight + 50, 160));
	titleScoreText.setString("Scores:");
	window.draw(titleScoreText);

	//show snakes scores
	int currentLine = 2;
	for (auto& snake : m_allPlayerSnakes) {
		//make player score text
		sf::Text playerSnakeScoreText;
		playerSnakeScoreText.setFont(m_font);
		playerSnakeScoreText.setCharacterSize(40);
		playerSnakeScoreText.setFillColor(snake.getColour());
		playerSnakeScoreText.setPosition(sf::Vector2f(m_windowHeight + 50, 160 + (40 * currentLine)));
		playerSnakeScoreText.setString(snake.getName() + ":\t" + std::to_string(snake.getScore()));
		window.draw(playerSnakeScoreText);
		currentLine++;
	}

	int snakeNum = 1;
	for (auto& snake : m_allAISnakes) {
		//make ai score text
		sf::Text aiSnakeScoreText;
		aiSnakeScoreText.setFont(m_font);
		aiSnakeScoreText.setCharacterSize(40);
		aiSnakeScoreText.setFillColor(snake.getColour());
		aiSnakeScoreText.setPosition(sf::Vector2f(m_windowHeight + 50, 160 + (40 * currentLine)));
		aiSnakeScoreText.setString(snake.getName() + ":\t" + std::to_string(snake.getScore()));
		window.draw(aiSnakeScoreText);
		currentLine++;
		snakeNum++;
	}

	//make food title text
	sf::Text titleFoodText;
	titleFoodText.setFont(m_font);
	titleFoodText.setCharacterSize(70);
	titleFoodText.setFillColor(sf::Color::White);
	titleFoodText.setPosition(sf::Vector2f(m_windowHeight + 50, 500));
	titleFoodText.setString("Items:");
	window.draw(titleFoodText);

	//show food type scores
	//make small food display
	sf::RectangleShape smallFood(sf::Vector2f(30, 30));
	smallFood.setPosition(sf::Vector2f(m_windowHeight + 50, 595));
	smallFood.setFillColor(sf::Color(255, 130, 236, 255));
	window.draw(smallFood);

	//make medium food display
	sf::RectangleShape mediumFood(sf::Vector2f(30, 30));
	mediumFood.setPosition(sf::Vector2f(m_windowHeight + 50,645));
	mediumFood.setFillColor(sf::Color(255, 64, 64, 255));
	window.draw(mediumFood);

	//make large food display
	sf::RectangleShape largeFood(sf::Vector2f(30, 30));
	largeFood.setPosition(sf::Vector2f(m_windowHeight + 50, 695));
	largeFood.setFillColor(sf::Color(182, 71, 255, 255));
	window.draw(largeFood);

	//make battery display
	sf::RectangleShape battery(sf::Vector2f(30, 30));
	battery.setPosition(sf::Vector2f(m_windowHeight + 50, 745));
	battery.setFillColor(sf::Color::Yellow);
	window.draw(battery);

	//make food point labels
	//small food label
	sf::Text smallFoodLabel;
	smallFoodLabel.setFont(m_font);
	smallFoodLabel.setCharacterSize(40);
	smallFoodLabel.setFillColor(sf::Color(255, 130, 236, 255));
	smallFoodLabel.setPosition(sf::Vector2f(900, 580));
	smallFoodLabel.setString(" = 10 pts");
	window.draw(smallFoodLabel);

	//medium food label
	sf::Text mediumFoodLabel;
	mediumFoodLabel.setFont(m_font);
	mediumFoodLabel.setCharacterSize(40);
	mediumFoodLabel.setFillColor(sf::Color(255, 64, 64, 255));
	mediumFoodLabel.setPosition(sf::Vector2f(900, 630));
	mediumFoodLabel.setString(" = 20 pts");
	window.draw(mediumFoodLabel);

	//large food label
	sf::Text largeFoodLabel;
	largeFoodLabel.setFont(m_font);
	largeFoodLabel.setCharacterSize(40);
	largeFoodLabel.setFillColor(sf::Color(182, 71, 255, 255));
	largeFoodLabel.setPosition(sf::Vector2f(900, 680));
	largeFoodLabel.setString(" = 30 pts");
	window.draw(largeFoodLabel);

	//battery label
	sf::Text batteryLabel;
	batteryLabel.setFont(m_font);
	batteryLabel.setCharacterSize(40);
	batteryLabel.setFillColor(sf::Color::Yellow);
	batteryLabel.setPosition(sf::Vector2f(900, 730));
	batteryLabel.setString(" = 50 pts");
	window.draw(batteryLabel);

	//make timer outline box
	sf::RectangleShape timerBox(sf::Vector2f(160, 80));
	timerBox.setPosition(sf::Vector2f(930, 52));
	timerBox.setFillColor(sf::Color::White);
	window.draw(timerBox);

	//make timer inside box
	sf::RectangleShape timerBoxInside(sf::Vector2f(152, 72));
	timerBoxInside.setPosition(sf::Vector2f(934, 56));
	timerBoxInside.setFillColor(sf::Color::Black);
	window.draw(timerBoxInside);

	//disply timer
	sf::Text timerLabel;
	timerLabel.setFont(m_font);
	timerLabel.setCharacterSize(70);
	timerLabel.setFillColor(sf::Color::White);
	timerLabel.setPosition(sf::Vector2f(970, 40));
	timerLabel.setString(std::to_string(m_maxTime - m_deltaGameTime));
	window.draw(timerLabel);
}

void Game::render(sf::RenderWindow& window) {

	if (m_menuActive) {
		makeMainMenu(window);
	}
	else {
		if (!m_gameOver) {
			//render player snakes
			for (auto& snake : m_allPlayerSnakes) {
				if (snake.getAlive()) {
					snake.render(window);
				}
			}
			//render ai snakes
			for (auto& snake : m_allAISnakes) {
				if (snake.getAlive()) {
					snake.render(window);
				}
			}

			//render food (if visible)
			for (auto& food : m_allFood) {
				food.render(window);
			}

			//render battery
			m_battery.render(window);

		}
		else {
			makeWinScreen(window);
		}

		//render ui
		makeUI(window);
	}
}