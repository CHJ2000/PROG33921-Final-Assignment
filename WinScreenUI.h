#ifndef WINSCREENUI_H
#define WINSCREENUI_H

#include <SFML/Graphics.hpp>
#include <string>



class WinScreenUI {
public:
	WinScreenUI(const sf::Font& font);

	void update(int score, float timeElapsed);
	void render(sf::RenderWindow& window);

private:
	sf::Text winText;
	sf::Text scoreText;
	sf::Text timeText;
};

#endif