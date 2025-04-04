#include "InGameUI.h"
#include <string>

InGameUI::InGameUI(sf::Font& font) {
	scoreText.setFont(font);
	scoreText.setCharacterSize(30);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(10.f, 10.f);

	timeText.setFont(font);
	timeText.setCharacterSize(30);
	timeText.setFillColor(sf::Color::White);
	timeText.setPosition(500.f, 10.f);
}

void InGameUI::update(int score, float time) {
	scoreText.setString("Score: " + std::to_string(score));
	timeText.setString("Time: " + std::to_string(static_cast<int>(time)));
}

void InGameUI::render(sf::RenderWindow& window) {
	window.draw(scoreText);
	window.draw(timeText);
}
