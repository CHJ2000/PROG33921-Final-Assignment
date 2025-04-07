#ifndef INGAMEUI_H
#define INGAMEUI_H

#include <SFML/Graphics.hpp>

class InGameUI {
public:
	InGameUI(sf::Font& font);
	void update(int score, float time, int playerHealth);
	void render(sf::RenderWindow& window);
	void initialize(sf::RenderWindow& window);

private:
	sf::Text scoreText;
	sf::Text timeText;
	sf::Text healthText;
};

#endif