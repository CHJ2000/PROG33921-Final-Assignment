#ifndef GAMEOVERMENU_H
#define GAMEOVERMENU_H

#include <SFML/Graphics.hpp>

class GameOverMenu {
public:
	GameOverMenu(sf::Font& font);
	void display(sf::RenderWindow& window, int finalScore, float finalTime);
	bool isRestartButtonClicked(const sf::RenderWindow& window);
	bool isExitButtonClicked(const sf::RenderWindow& window);

private:
	sf::Text gameOverText;
	sf::Text scoreText;
	sf::Text timeText;
	sf::Text restartButton;
	sf::Text exitButton;
};

#endif 