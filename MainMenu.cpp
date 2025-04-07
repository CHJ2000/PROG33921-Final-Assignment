#include "MainMenu.h"
#include <iostream>

MainMenu::MainMenu(sf::Font& font) {
	titleText.setFont(font);
	titleText.setString("Welcome to Wizards n' Werewolves");
	titleText.setFillColor(sf::Color::Magenta);
	titleText.setCharacterSize(70);

	startButton.setFont(font);
	startButton.setString("Start");
	startButton.setCharacterSize(50);
	startButton.setFillColor(sf::Color::White);
	

	exitButton.setFont(font);
	exitButton.setString("Exit");
	exitButton.setCharacterSize(50);
	exitButton.setFillColor(sf::Color::White);
	
}

void MainMenu::initialize(sf::RenderWindow& window) {
	float windowWidth = static_cast<float>(window.getSize().x);
	float windowHeight = static_cast<float>(window.getSize().y);

	sf::FloatRect titleBounds = titleText.getLocalBounds();
	titleText.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top + titleBounds.height / 2.0f);
	titleText.setPosition(windowWidth / 2.0f, windowHeight * 0.2f);

	sf::FloatRect startBounds = startButton.getLocalBounds();
	startButton.setOrigin(startBounds.left + startBounds.width / 2.0f, startBounds.top + startBounds.height / 2.0f);
	startButton.setPosition(windowWidth / 2.0f, windowHeight * 0.4f);

	sf::FloatRect exitBounds = exitButton.getLocalBounds();
	exitButton.setOrigin(exitBounds.left + exitBounds.width / 2.0f, exitBounds.top + exitBounds.height / 2.0f);
	exitButton.setPosition(windowWidth / 2.0f, windowHeight * 0.5f);


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