#include "InGameUI.h"
#include <string>

InGameUI::InGameUI(sf::Font& font) {
	scoreText.setFont(font);
	scoreText.setString("Score: 0");
	scoreText.setCharacterSize(40);
	scoreText.setFillColor(sf::Color(255, 223, 0));

	timeText.setFont(font);
	timeText.setString("Time: 0s");
	timeText.setCharacterSize(40);
	timeText.setFillColor(sf::Color(255, 223, 0));
}

void InGameUI::initialize(sf::RenderWindow& window) {
	float windowWidth = static_cast<float>(window.getSize().x);
	float windowHeight = static_cast<float>(window.getSize().y);

	const float paddingX = static_cast<float>(windowWidth * 0.05);
	const float paddingY = static_cast<float>(windowHeight * 0.05);

	sf::FloatRect scoreBounds = scoreText.getGlobalBounds();
	scoreText.setPosition(
		paddingX, paddingY
	);

	sf::FloatRect timeBounds = timeText.getGlobalBounds();
	timeText.setPosition(
		windowWidth - timeBounds.width - paddingX, paddingY
	);
}

void InGameUI::update(int score, float timeElapsed) {
	scoreText.setString("Score: " + std::to_string(score));
	timeText.setString("Time: " + std::to_string(static_cast<int>(timeElapsed)) + "s");
}

void InGameUI::render(sf::RenderWindow& window) {
	sf::View uiView = window.getDefaultView();
	window.setView(uiView);
	window.draw(scoreText);
	window.draw(timeText);
}
