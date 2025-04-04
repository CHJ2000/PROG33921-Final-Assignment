#include "GameManager.h"
#include <algorithm>
#include <fstream>

GameManager::GameManager()
	: window(sf::VideoMode(1000, 800), "Wizards 'n Werewolves"), player(100.f, 500.f) {
	window.setFramerateLimit(60);

	std::ifstream file("Debug/assets/fonts/BlackwoodCastle.ttf");
	if (!file) {
		std::cerr << "Font file not found at the specified path!" << std::endl;
	}
	else {
		std::cerr << "Font file successfully found!" << std::endl;
	}
	file.close();

	if (!font.loadFromFile("Debug/assets/fonts/BlackwoodCastle.ttf")) {
		std::cerr << "Failed to load font!" << std::endl;
	}


	mainMenuUI = new MainMenu(font);
	inGameUI = new InGameUI(font);
	gameOverUI = new GameOverMenu(font);

	camera.setSize(1200.f, 900.f);
	camera.setCenter(player.getShape().getPosition());

	enemies.emplace_back(600.f, 500.f, 800.f);
	enemies.emplace_back(1200.f, 500.f, 1400.f);
	enemies.emplace_back(1800.f, 500.f, 2000.f);

	ground.setSize(sf::Vector2f(2000.f, 50.f));
	ground.setFillColor(sf::Color::Green);
	ground.setPosition(0.f, 550.f);

	obstacle1.setSize(sf::Vector2f(50.f, 50.f));
	obstacle1.setFillColor(sf::Color::Yellow);
	obstacle1.setPosition(300.f, 500.f);


	obstacle2.setSize(sf::Vector2f(50.f, 50.f));
	obstacle2.setFillColor(sf::Color::Yellow);
	obstacle2.setPosition(1100.f, 500.f);
}

void GameManager::run() {
	while (window.isOpen()) {
		mainMenu();
		gameplay();
		gameOver();
	}
}


void GameManager::mainMenu() {
	mainMenuUI->initialize(window);
	while (window.isOpen()) {
		window.clear();
		mainMenuUI->display(window);

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed) {
				if (mainMenuUI->isStartButtonClicked(window)) {
					gameClock.restart();
					return;
				}
				if (mainMenuUI->isExitButtonClicked(window)) {
					window.close();
				}
			}
		}
	}
}

void GameManager::gameplay() {
	while (window.isOpen()) {
		float deltaTime = gameClock.restart().asSeconds();

		handleEvents();
		update(deltaTime);
		render();

		if (player.getHealth() <= 0) {
			gameOver();
			return;
		}
	}
}

void GameManager::gameOver() {
	while (window.isOpen()) {
		window.clear(sf::Color::Red);
		gameOverUI->display(window, score, gameClock.getElapsedTime().asSeconds());

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed) {
				if (gameOverUI->isRestartButtonClicked(window)) {
					score = 0;
					player.reset();
					gameClock.restart();
					return;
				}
				if (gameOverUI->isExitButtonClicked(window)) {
					window.close();
				}
			}
		}
	}
}
void GameManager::handleEvents() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window.close();

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
			player.attack(projectiles);
		}
	}
}

void GameManager::update(float deltaTime) {
	sf::Vector2f movement(0.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		movement.x -= 1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		movement.x += 1.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		player.jump();

	player.move(movement, deltaTime, 200.f);
	player.update(deltaTime);

	const sf::FloatRect playerBounds = player.getShape().getGlobalBounds();


	const sf::FloatRect obstacle1Bounds = obstacle1.getGlobalBounds();
	if (playerBounds.intersects(obstacle1Bounds)) {
		if (playerBounds.top + playerBounds.height <= obstacle1Bounds.top + 10.f) {
			player.getShape().setPosition(playerBounds.left, obstacle1Bounds.top - playerBounds.height);
			player.stopFalling();
		}
		else {
			if (movement.x > 0.f && playerBounds.left + playerBounds.width > obstacle1Bounds.left) {
				player.getShape().setPosition(obstacle1Bounds.left - playerBounds.width, playerBounds.top);
			}
			if (movement.x < 0.f && playerBounds.left < obstacle1Bounds.left + obstacle1Bounds.width) {
				player.getShape().setPosition(obstacle1Bounds.left + obstacle1Bounds.width, playerBounds.top);
			}
		}
	}


	for (auto& enemy : enemies) {
		enemy.update(player.getShape(), deltaTime, obstacle1);
	}

	for (auto& projectile : projectiles) {
		projectile.update(deltaTime);
	}

	checkCollisions();

	camera.setCenter(player.getShape().getPosition());
	window.setView(camera);
}

void GameManager::render() {
	window.clear();

	window.draw(ground);
	window.draw(player.getShape());

	for (const auto& enemy : enemies) {
		window.draw(enemy.getShape());
	}
	window.draw(obstacle1);
	window.draw(obstacle2);
	for (const auto& projectile : projectiles) {
		window.draw(projectile.getShape());
	}

	float timeElapsed = gameClock.getElapsedTime().asSeconds();
	inGameUI->update(score, timeElapsed);
	inGameUI->render(window);
	window.display();
}

void GameManager::checkCollisions() {
	for (auto it = projectiles.begin(); it != projectiles.end();) {
		const sf::FloatRect projectileBounds = it->getShape().getGlobalBounds();
		if (projectileBounds.intersects(obstacle1.getGlobalBounds()) || projectileBounds.intersects(obstacle2.getGlobalBounds())) {
			it = projectiles.erase(it);
		}
		else {
			++it;
		}
	}

	for (auto& enemy : enemies) {
		if (player.getShape().getGlobalBounds().intersects(enemy.getShape().getGlobalBounds())) {
			player.takeDamage();
			if (player.getHealth() <= 0) {
				gameOver();
				return;
			}
		}
	}

	for (auto& enemy : enemies) {
		for (auto it = projectiles.begin(); it != projectiles.end();) {
			if (it->getShape().getGlobalBounds().intersects(enemy.getShape().getGlobalBounds())) {
				enemy.takeDamage();
				it = projectiles.erase(it);
			}
			else {
				++it;
			}
		}
	}
	enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const Werewolf& enemy) {
		return !enemy.isAlive();
	}), enemies.end());
}
