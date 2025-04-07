#include "GameManager.h"
#include <algorithm>
#include <fstream>

GameManager::GameManager()
	: window(sf::VideoMode(1000, 600), "Wizards 'n Werewolves"), player(100.f, 500.f) {
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
	currentState = GameState::MainMenu;

	camera.setSize(1200.f, 900.f);
	camera.setCenter(player.getShape().getPosition());

	//enemies.emplace_back(600.f, 500.f, 800.f);
	//enemies.emplace_back(1200.f, 500.f, 1400.f);
	//enemies.emplace_back(1800.f, 500.f, 2000.f);

	ground.setSize(sf::Vector2f(2000.f, 50.f));
	ground.setFillColor(sf::Color::Green);
	ground.setPosition(0.f, 550.f);

}

void GameManager::run() {
	while (window.isOpen()) {

		window.clear(); 

		
		if (currentState == GameState::MainMenu) {
			mainMenuUI->initialize(window); 
			mainMenu();                  
		}
		else if (currentState == GameState::Playing) {
			gamePlay(); 
		}
		else if (currentState == GameState::GameOver) {
			gameOver(); 
		}

		window.display(); 
	}
}


void GameManager::mainMenu() {
	handleMainMenuEvents();
	renderMainMenu();
	
	}

void GameManager::handleMainMenuEvents() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
			return;
		}
		if (event.type == sf::Event::MouseButtonPressed) {
			if (mainMenuUI->isStartButtonClicked(window)) {
				gameClock.restart();
				currentState = GameState::Playing;
				return;
			}
			if (mainMenuUI->isExitButtonClicked(window)) {
				window.close();
				return;
			}
		}
	}
}

void GameManager::renderMainMenu() {
	window.clear();             
	mainMenuUI->display(window); 
	window.display();
}

void GameManager::gamePlay() {
	inGameUI->initialize(window);
	gameClock.restart();
	sf::Clock frameClock;

	while (window.isOpen()) {
		float deltaTime = frameClock.restart().asSeconds();

		handleEvents();
		update(deltaTime);
		render();
		
		if (player.getHealth() <= 0) {
			finalTime = gameClock.getElapsedTime().asSeconds();
			currentState = GameState::GameOver;
			return;

		}
	}
}

void GameManager::gameOver() {
	handleGameOverEvents();
	renderGameOver();
	}

void GameManager::handleGameOverEvents() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
			return;
		}
		if (event.type == sf::Event::MouseButtonPressed) {
			if (gameOverUI->isRestartButtonClicked(window)) {
				score = 0;
				player.reset();
				gameClock.restart();
				currentState = GameState::Playing;
				return;
			}
			if (gameOverUI->isExitButtonClicked(window)) {
				window.close();
				return;
			}
		}
	}
}

void GameManager::renderGameOver() {
	window.clear();              
	gameOverUI->display(window, score, finalTime);
	window.display();
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

	updateGround(player.getShape().getPosition().x);
	spawnEntities(player.getShape().getPosition().x);
	cleanUpEntities(player.getShape().getPosition().x);


	for (auto& enemy : enemies) {
		enemy.update(player.getShape(), deltaTime, obstacles);
	}

	for (auto& projectile : projectiles) {
		projectile.update(deltaTime);
	}

	checkCollisions();

	camera.setCenter(player.getShape().getPosition());
	window.setView(camera);
}

void GameManager::spawnEntities(float playerX) {
	const float respawnThreshold = 500.f;
	const float obstacleWidth = 50.f;
	const float obstacleHeight = 50.f;

	if (obstacles.empty() || playerX > obstacles.back().getShape().getPosition().x + respawnThreshold) {
		float obstacleX = obstacles.empty() ? playerX + 500.f : obstacles.back().getShape().getPosition().x + respawnThreshold;
		float obstacleY = ground.getPosition().y - 50.f;
		Obstacle newObstacle(obstacleX, obstacleY, obstacleWidth, obstacleHeight);
		obstacles.push_back(newObstacle);
		std::cerr << "Obstacle spawned at: " << newObstacle.getShape().getPosition().x << ", " << newObstacle.getShape().getPosition().y << std::endl;
	}
	if (enemies.empty() || playerX > enemies.back().getShape().getPosition().x + respawnThreshold) {
		float enemyX = enemies.empty() ? playerX + 400.f : enemies.back().getShape().getPosition().x + respawnThreshold;
		float patrolStartX = enemyX - 100.f;
		float patrolEndX = enemyX + 100.f;
		Werewolf newEnemy(patrolStartX, 500.f, patrolEndX);
		enemies.push_back(newEnemy);
		std::cerr << "Enemy spawned at: " << enemyX << ", 500" << std::endl;
	}
}

void GameManager::cleanUpEntities(float playerX) {
	const float cleanupThreshold = 400.f;

	obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(),
		[playerX, cleanupThreshold](const Obstacle& obstacle) {
			return obstacle.getShape().getPosition().x < playerX - cleanupThreshold;
		}), obstacles.end());

	enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
		[playerX, cleanupThreshold](const Werewolf& enemy) {
			return enemy.getShape().getPosition().x < playerX - cleanupThreshold;
		}), enemies.end());
}

void GameManager::updateGround(float playerX) {
	const float extensionThreshold = 500.f;

	if (playerX > ground.getPosition().x + ground.getSize().x - extensionThreshold) {
		ground.setSize(sf::Vector2f(ground.getSize().x + extensionThreshold, ground.getSize().y));
	}
}

void GameManager::render() {
	window.clear();
	window.setView(camera);

	window.draw(ground);
	window.draw(player.getShape());

	for (const auto& obstacle : obstacles) {
		window.draw(obstacle.getShape());
	}

	for (const auto& enemy : enemies) {
		window.draw(enemy.getShape());
	}
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
		
		bool collisionDetected = false;
		for (const auto& obstacle : obstacles) {
			if (projectileBounds.intersects(obstacle.getShape().getGlobalBounds())) {
				collisionDetected = true;
				break;
			}
		}
			if (collisionDetected){
				it = projectiles.erase(it);
			}else {
				++it;
		}
	}

	const sf::FloatRect playerBounds = player.getShape().getGlobalBounds();
	for (const auto& obstacle : obstacles) {
		if (playerBounds.intersects(obstacle.getShape().getGlobalBounds())) {
			if (playerBounds.top + playerBounds.height <= obstacle.getShape().getPosition().y + 10.f) {
				// Landing on the obstacle
				player.getShape().setPosition(playerBounds.left, obstacle.getShape().getPosition().y - playerBounds.height);
				player.stopFalling(); // Stop the player from falling further
			}
			else {
				// Colliding from the sides
				if (player.getVelocity().x > 0.f && playerBounds.left + playerBounds.width > obstacle.getShape().getPosition().x) {
					// Collision from the right
					player.getShape().setPosition(obstacle.getShape().getPosition().x - playerBounds.width, playerBounds.top);
				}
				else if (player.getVelocity().x < 0.f && playerBounds.left < obstacle.getShape().getPosition().x + obstacle.getShape().getSize().x) {
					// Collision from the left
					player.getShape().setPosition(obstacle.getShape().getPosition().x + obstacle.getShape().getSize().x, playerBounds.top);
				}
			}
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
