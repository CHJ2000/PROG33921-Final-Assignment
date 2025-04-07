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

	healthText.setFont(font);
	healthText.setString("Health: 3");
	healthText.setCharacterSize(40); 
	healthText.setFillColor(sf::Color(255, 0, 0));

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

	
	sf::FloatRect healthBounds = healthText.getGlobalBounds();
	healthText.setPosition(
		paddingX, paddingY + scoreBounds.height + 10.f 
	);

}

void InGameUI::update(int score, float timeElapsed, int playerHealth) {
	scoreText.setString("Score: " + std::to_string(score));
	timeText.setString("Time: " + std::to_string(static_cast<int>(timeElapsed)) + "s");
	healthText.setString("Health: " + std::to_string(playerHealth));
}

void InGameUI::render(sf::RenderWindow& window) {
	sf::View uiView = window.getDefaultView();
	window.setView(uiView);
	window.draw(scoreText);
	window.draw(timeText);
	window.draw(healthText);
}
