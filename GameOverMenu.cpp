#include "GameOverMenu.h"

GameOverMenu::GameOverMenu(sf::Font& font) {
	gameOverText.setFont(font);
	gameOverText.setString("Game Over");
	gameOverText.setCharacterSize(70);
	gameOverText.setFillColor(sf::Color::Red);
	gameOverText.setPosition(200.f, 100.f);

	scoreText.setFont(font);
	scoreText.setCharacterSize(50);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(200.f, 250.f);

	restartButton.setFont(font);
	restartButton.setString("Restart");
	restartButton.setCharacterSize(50);
	restartButton.setFillColor(sf::Color::White);
	restartButton.setPosition(200.f, 500.f);
}

void GameOverMenu::display(sf::RenderWindow& window, int finalScore, float finalTime) {
	scoreText.setString("Final Score: " + std::to_string(finalScore));
	sf::Text timeText;
	timeText.setFont(*gameOverText.getFont());
	timeText.setCharacterSize(50);
	timeText.setFillColor(sf::Color::White);
	timeText.setString("Final Time: " + std::to_string(static_cast<int>(finalTime)) + "s");
	timeText.setPosition(200.f, 320.f);


	window.clear();
	window.draw(gameOverText);
	window.draw(scoreText);
	window.draw(timeText);
	window.draw(restartButton);
	window.draw(exitButton);
	window.display();
}

bool GameOverMenu::isRestartButtonClicked(const sf::RenderWindow& window) {
	return restartButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
}

bool GameOverMenu::isExitButtonClicked(const sf::RenderWindow& window) {
	return exitButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
}
