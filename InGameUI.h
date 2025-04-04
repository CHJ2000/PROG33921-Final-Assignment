#ifndef INGAMEUI_H
#define INGAMEUI_H

#include <SFML/Graphics.hpp>

class InGameUI {
public:
	InGameUI(sf::Font& font);
	void update(int score, float time);
	void render(sf::RenderWindow& window);

private:
	sf::Text scoreText;
	sf::Text timeText;
};

#endif