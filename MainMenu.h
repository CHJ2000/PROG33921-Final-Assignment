#ifndef MAINMENU_H
#define MAINMENU_H

#include <SFML/Graphics.hpp>

class MainMenu {
public:
	MainMenu(sf::Font& font);
	void display(sf::RenderWindow& window);
	void initialize(sf::RenderWindow& window);
	bool isStartButtonClicked(const sf::RenderWindow& window);
	bool isExitButtonClicked(const sf::RenderWindow& window);

private:
	sf::Text titleText;
	sf::Text startButton;
	sf::Text exitButton;
};

#endif