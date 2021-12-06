/*
  Games Development with C++ GAV1031-N
  Snake ICA start project 
  Thomas Coulson A0050289

  Set up to use the SFML 2.5.1 64-bit API
    SFML documentation: https://www.sfml-dev.org/learn.php
*/
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include "Game.h"

int main()
{
    srand(time(NULL));
    
	// move these into a globals namespace
	// mark as const
    int windowWidth = 1200;
    int windowHeight = 820;
    int gameWidth = windowHeight;
    int gameHeight = windowHeight;
    int gridSize = 20;
    int leftMidPoint = (((gameHeight / gridSize) / 2) + 1) * (gridSize);
    int startingLength = 5;

    //make game window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "C++ Snake ICA : A0050289");

    //make the game
    Game newGame(gameWidth, gameHeight, startingLength, gridSize, leftMidPoint);

    //make the snakes
    newGame.makeGameObjects();

    //game clock
    newGame.restartGameTime();
    sf::Clock gameClock;

    while (window.isOpen())
    {
        //check if user closes the game window
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            }
        }

        //restart game if space is pressed during game over screen
        if (newGame.getGameOver() && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            
            //create a new game instance to reset the game
            Game startNewGame(gameWidth, gameHeight, startingLength, gridSize, leftMidPoint);
           
            startNewGame.makeGameObjects();

            startNewGame.restartGameTime();

            newGame = startNewGame;

        }

        //update the game every 75 milliseconds
        float deltaTime = gameClock.getElapsedTime().asMilliseconds();
        if (deltaTime > 75.f) {

            //update game
            newGame.update();

            window.clear();

            //check if any snakes die
            newGame.checkDeadSnakes();

            //check game over
            newGame.checkGameOver();

            //render game
            newGame.render(window);

            window.display();

            gameClock.restart();
        }

    }

    return 0;
}
