#include "GameManager.h"
#include <algorithm>

GameManager::GameManager()
	: window(sf::VideoMode(800, 600), "Ghosts 'n Goblins"), player(100.f, 500.f) {
	window.setFramerateLimit(60);

	camera.setSize(800.f, 600.f);
	camera.setCenter(player.getShape().getPosition());

	enemies.emplace_back(600.f, 500.f, 800.f);
	enemies.emplace_back(1200.f, 500.f, 1400.f);
	enemies.emplace_back(1800.f, 500.f, 2000.f);

	ground.setSize(sf::Vector2f(2000.f, 50.f));
	ground.setFillColor(sf::Color::Green);
	ground.setPosition(0.f, 550.f);

	obstacle1.setSize(sf::Vector2f(50.f, 50.f));
	obstacle1.setFillColor(sf::Color::Red);
	obstacle1.setPosition(300.f, 500.f);


	obstacle2.setSize(sf::Vector2f(50.f, 50.f));
	obstacle2.setFillColor(sf::Color::Red);
	obstacle2.setPosition(1100.f, 500.f);
}

void GameManager::run() {
	sf::Clock clock;

	while (window.isOpen()) {
		sf::Time deltaTime = clock.restart();

		handleEvents();
		update(deltaTime.asSeconds());
		render();
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

	for (auto& enemy : enemies) {
		enemy.update(player.getShape(), deltaTime);
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
	window.display();
}

void GameManager::checkCollisions() {
	for (auto& enemy : enemies) {
		if (player.getShape().getGlobalBounds().intersects(enemy.getShape().getGlobalBounds())) {
			window.close();
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
