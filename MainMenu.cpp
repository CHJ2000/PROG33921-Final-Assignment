#include "MainMenu.h"

MainMenu::MainMenu(sf::Font& font) {
	titleText.setFont(font);
	titleText.setString("Welcome to Wizards n' Werewolves");
	titleText.setFillColor(sf::Color::Magenta);
	titleText.setCharacterSize(70);

	startButton.setFont(font);
	startButton.setString("Start");
	startButton.setCharacterSize(50);
	startButton.setFillColor(sf::Color::White);
	/*startButton.setPosition(300.f, 200.f);*/

	exitButton.setFont(font);
	exitButton.setString("Exit");
	exitButton.setCharacterSize(50);
	exitButton.setFillColor(sf::Color::White);
	/*exitButton.setPosition(300.f, 300.f);*/
}

void MainMenu::initialize(sf::RenderWindow& window) {
	float windowWidth = static_cast<float>(window.getSize().x);
	float windowHeight = static_cast<float>(window.getSize().y);

	sf::FloatRect titleBounds = titleText.getGlobalBounds();
	titleText.setPosition(
		static_cast<float>((windowWidth - titleBounds.width) / 2.0),
			static_cast<float>(windowHeight * 0.2)
			);

	sf::FloatRect startBounds = startButton.getGlobalBounds();
	startButton.setPosition(
		static_cast<float>((windowWidth - startBounds.width) / 2.0),
		static_cast<float>(windowHeight * 0.4)
	);

	sf::FloatRect exitBounds = exitButton.getGlobalBounds();
	exitButton.setPosition(
		static_cast<float>((windowWidth - exitBounds.width) / 2.0),
		static_cast<float>(windowHeight * 0.5)
	);

}

void MainMenu::display(sf::RenderWindow& window) {
	window.clear(sf::Color(10, 10, 50));
	window.draw(titleText);
	window.draw(startButton);
	window.draw(exitButton);
	window.display();
}

bool MainMenu::isStartButtonClicked(const sf::RenderWindow& window) {
	return startButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
}

bool MainMenu::isExitButtonClicked(const sf::RenderWindow& window) {
	return exitButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
}