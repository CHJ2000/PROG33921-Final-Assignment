#include "GameOverMenu.h"
#include <iostream>

GameOverMenu::GameOverMenu(sf::Font& font) {
	gameOverText.setFont(font);
	gameOverText.setString("Game Over");
	gameOverText.setCharacterSize(70);
	gameOverText.setFillColor(sf::Color::Black);
	/*gameOverText.setPosition(200.f, 100.f);*/

	scoreText.setFont(font);
	scoreText.setCharacterSize(50);
	scoreText.setFillColor(sf::Color::White);
	/*scoreText.setPosition(200.f, 250.f);*/

	restartButton.setFont(font);
	restartButton.setString("Restart");
	restartButton.setCharacterSize(50);
	restartButton.setFillColor(sf::Color::White);
	/*restartButton.setPosition(200.f, 500.f);*/

	exitButton.setFont(font);
	exitButton.setString("Exit");
	exitButton.setCharacterSize(50);
	exitButton.setFillColor(sf::Color::White);
	/*exitButton.setPosition(450.f, 500.f);*/

	timeText.setFont(font);
	timeText.setCharacterSize(50);
	timeText.setFillColor(sf::Color::White);
	/*timeText.setPosition(200.f, 320.f);*/
}

void GameOverMenu::display(sf::RenderWindow& window, int finalScore, float finalTime) {
	scoreText.setString("Final Score: " + std::to_string(finalScore));
	timeText.setString("Final Time: " + std::to_string(static_cast<int>(finalTime)) + "s");
	
	float windowWidth = static_cast<float>(window.getSize().x);

	sf::FloatRect gameOverBounds = gameOverText.getGlobalBounds();
	gameOverText.setPosition((windowWidth - gameOverBounds.width) / 2.f, 100.f);

	sf::FloatRect scoreBounds = scoreText.getGlobalBounds();
	scoreText.setPosition((windowWidth - scoreBounds.width) / 2.f, 250.f);

	sf::FloatRect timeBounds = timeText.getGlobalBounds();
	timeText.setPosition((windowWidth - timeBounds.width) / 2.f, 320.f);

	sf::FloatRect restartBounds = restartButton.getGlobalBounds();
	restartButton.setPosition((windowWidth - restartBounds.width) / 2.f - 150.f, 500.f);

	sf::FloatRect exitBounds = exitButton.getGlobalBounds();
	exitButton.setPosition((windowWidth - exitBounds.width) / 2.f + 150.f, 500.f);

	window.clear(sf::Color::Red);
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
