#include "WinScreenUI.h"
#include <iostream>

WinScreenUI::WinScreenUI(const sf::Font& font) {
	winText.setFont(font);
	winText.setString("Level 1 Completed!");
	winText.setCharacterSize(70);
	winText.setFillColor(sf::Color::Green);
	winText.setPosition(300.f, 200.f);

	scoreText.setFont(font);
	scoreText.setCharacterSize(50);
	scoreText.setFillColor(sf::Color::Yellow);
	scoreText.setPosition(300.f, 300.f);

	timeText.setFont(font);
	timeText.setCharacterSize(50);
	timeText.setFillColor(sf::Color::Yellow);
	timeText.setPosition(300.f, 350.f);
}

void WinScreenUI::update(int score, float timeElapsed) {
	scoreText.setString("Final Score: " + std::to_string(score));
	timeText.setString("Final Time: " + std::to_string(static_cast<int>(timeElapsed)) + " seconds");
}

void WinScreenUI::render(sf::RenderWindow& window) {
	std::cerr << "Drawing Win Screen elements..." << std::endl;
	window.draw(winText);
	window.draw(scoreText);
	window.draw(timeText);
}