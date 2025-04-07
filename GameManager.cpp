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

	ground.setSize(sf::Vector2f(2000.f, 50.f));
	ground.setFillColor(sf::Color::Green);
	ground.setPosition(0.f, 550.f);

	score = 0;

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

	spawnEntities(player.getShape().getPosition().x);

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
	const float obstacleWidth = 50.f;
	const float obstacleHeight = 50.f;
	const float spacing = 600.f;
	const float offSceenDistance = 1500.f;

	while (obstacles.size() < 2 || obstacles.back().getShape().getPosition().x < playerX + offSceenDistance) {
		float obstacleX = obstacles.empty()
			? playerX + offSceenDistance :
			obstacles.back().getShape().getPosition().x + spacing;
		float obstacleY = ground.getPosition().y - obstacleHeight;

		Obstacle newObstacle(obstacleX, obstacleY, obstacleWidth, obstacleHeight);
		obstacles.push_back(newObstacle);
		std::cerr << "Obstacle spawned off-screen at: " << obstacleX << ", " << obstacleY << std::endl;

		Obstacle* firstObstacle = nullptr;
		Obstacle* secondObstacle = nullptr;

		for(size_t i = 0; i < obstacles.size() - 1; ++i) {
			if (obstacles[i].getShape().getPosition().x > playerX) {
				firstObstacle = &obstacles[i];
				secondObstacle = &obstacles[i + 1];
				break;
			}
		}
		if (firstObstacle && secondObstacle && enemies.empty()) {
			float enemyX = (firstObstacle->getShape().getPosition().x + secondObstacle->getShape().getPosition().x) / 2.f;
			float patrolStartX = firstObstacle->getShape().getPosition().x + obstacleWidth;
			float patrolEndX = secondObstacle->getShape().getPosition().x - obstacleWidth;

			Werewolf newEnemy(patrolStartX, 500.f, patrolEndX);
			enemies.push_back(newEnemy);
			std::cerr << "Enemy spawned at (off-screen): " << enemyX
				<< " with patrol between " << patrolStartX << " and " << patrolEndX << std::endl;
			}

		}
	}
void GameManager::cleanUpEntities(float playerX) {
	const float cleanupThreshold = 800.f;

	obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(),
		[playerX, cleanupThreshold](const Obstacle& obstacle) {
			return obstacle.getShape().getPosition().x < playerX - cleanupThreshold;
		}), obstacles.end());

	enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
		[playerX, cleanupThreshold](const Werewolf& enemy) {
			return enemy.getShape().getPosition().x < playerX - cleanupThreshold && enemy.isAlive();
		}), enemies.end());
}

void GameManager::updateGround(float playerX) {
	const float extensionThreshold = 1500.f;

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
		const sf::FloatRect obstacleBounds = obstacle.getShape().getGlobalBounds();

		if (playerBounds.intersects(obstacleBounds)) {
			if (playerBounds.top + playerBounds.height <= obstacleBounds.top + 10.f &&
				player.getVelocity().y > 0) {
				player.getShape().setPosition(playerBounds.left, obstacleBounds.top - playerBounds.height);
				player.stopFalling();
			}
			else if (playerBounds.left + playerBounds.width > obstacleBounds.left && 
					playerBounds.left < obstacleBounds.left && 
					playerBounds.top + playerBounds.height > obstacleBounds.top &&
					playerBounds.top < obstacleBounds.top + obstacleBounds.height)
				 {
					player.getShape().setPosition(obstacleBounds.left - playerBounds.width - 0.1f, playerBounds.top);
					player.stopMovingHorizontally();
				 }
				else if (playerBounds.left < obstacleBounds.left + obstacleBounds.width && 
						playerBounds.left + playerBounds.width > obstacleBounds.left + obstacleBounds.width && 
						playerBounds.top + playerBounds.height > obstacleBounds.top &&
						playerBounds.top < obstacleBounds.top + obstacleBounds.height) 
				{
					player.getShape().setPosition(obstacleBounds.left + obstacleBounds.width + 0.1f, playerBounds.top);
					player.stopMovingHorizontally();
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
