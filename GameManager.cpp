#include "GameManager.h"
#include <algorithm>
#include <fstream>
#include "Boss.h"

GameManager::GameManager()
	: window(sf::VideoMode(1000, 600), "Wizards 'n Werewolves"), player(100.f, 500.f), bossSpawned(false) {
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

	if (!obstacleTexture.loadFromFile("Debug/assets/sprites/tombstone.png")) {
		std::cerr << "Failed to load obstacle texture!" << std::endl;
	}
	else {
		std::cerr << "Obstacle texture loaded successfully!" << std::endl;
	}

	if (!obstacleTexture.loadFromFile("Debug/assets/sprites/tombstone.png")) {
		std::cerr << "Failed to load obstacle texture!" << std::endl;
	}

	if (!projectileTexture.loadFromFile("Debug/assets/sprites/magicProjectile.png")) {
		std::cerr << "Failed to load projectile texture!" << std::endl;
	}
	else {
		std::cerr << "Projectile texture loaded successfully!" << std::endl;
	}



	mainMenuUI = new MainMenu(font);
	inGameUI = new InGameUI(font);
	gameOverUI = new GameOverMenu(font);
	winScreenUI = new WinScreenUI(font);
	currentState = GameState::MainMenu;

	camera.setSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
	camera.setCenter(player.getShape().getPosition().x, static_cast<float>(window.getSize().y) / 2.f);

	ground.setSize(sf::Vector2f(window.getSize().x * 2.f, 50.f));
	ground.setFillColor(sf::Color::Green);
	ground.setPosition(0.f, static_cast<float>(window.getSize().y) - ground.getSize().y);

	score = 0;
	finalScore = 0;
	finalTime = 0.f;

}

GameManager::~GameManager() {
	delete mainMenuUI;
	delete inGameUI;
	delete gameOverUI;
	delete winScreenUI; 
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

	initializeEntities(player.getShape().getPosition().x);
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
				resetGame();
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
	window.clear(sf::Color::Red);              
	gameOverUI->display(window, score, finalTime);
	window.display();
}

void GameManager::handleEvents() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window.close();

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
			player.attack(projectiles, projectileTexture);
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

	if (currentState == GameState::Playing && score >= 500 && !bossSpawned) {
		spawnBoss(player.getShape().getPosition().x);
		bossSpawned = true;
	}

	for (auto& boss : bosses) {
		boss.move(deltaTime, player.getShape().getPosition(), obstacles);
	}

	checkCollisions();
	
	float timeElapsed = gameClock.getElapsedTime().asSeconds();
	inGameUI->update(score, timeElapsed, player.getHealth());

	camera.setCenter(player.getShape().getPosition());
	window.setView(camera);
}

void GameManager::initializeEntities(float playerX) {
	const float obstacleWidth = 50.f;
	const float obstacleHeight = 50.f;
	const float spacing = 600.f;

	sf::FloatRect cameraBounds(camera.getCenter().x - camera.getSize().x / 2,
		camera.getCenter().y - camera.getSize().y / 2,
		camera.getSize().x,
		camera.getSize().y);

	float obstacleX1 = playerX + 400.f;
	float obstacleY1 = ground.getPosition().y - obstacleHeight;
	Obstacle staticObstacle1(obstacleX1, obstacleY1, obstacleWidth, obstacleHeight, obstacleTexture);
	obstacles.push_back(staticObstacle1);

	float obstacleX2 = obstacleX1 + spacing;
	float obstacleY2 = ground.getPosition().y - obstacleHeight;
	Obstacle staticObstacle2(obstacleX2, obstacleY2, obstacleWidth, obstacleHeight, obstacleTexture);
	obstacles.push_back(staticObstacle2);

	if (enemies.empty()) {
		float enemyX = (obstacles[0].getSprite().getPosition().x + obstacles[1].getSprite().getPosition().x) / 2.f;
		float patrolStartX = obstacles[0].getSprite().getPosition().x + obstacleWidth;
		float patrolEndX = obstacles[1].getSprite().getPosition().x - obstacleWidth;

		Werewolf staticEnemy(patrolStartX, 500.f, patrolEndX);
		enemies.push_back(staticEnemy);
	}
}

void GameManager::spawnEntities(float playerX) {
	const float obstacleWidth = 50.f;
	const float obstacleHeight = 50.f;
	const float spacing = 800.f;
	const float offSceenDistance = 1500.f;

	sf::FloatRect cameraBounds(camera.getCenter().x - camera.getSize().x / 2,
		camera.getCenter().y - camera.getSize().y / 2,
		camera.getSize().x,
		camera.getSize().y);

	while (obstacles.size() < 2 || obstacles.back().getSprite().getPosition().x < playerX + offSceenDistance) {
		float obstacleX = obstacles.empty()
			? playerX + offSceenDistance :
			obstacles.back().getSprite().getPosition().x + spacing;
		float obstacleY = ground.getPosition().y - obstacleHeight;

		Obstacle newObstacle(obstacleX, obstacleY, obstacleWidth, obstacleHeight, obstacleTexture);
		obstacles.push_back(newObstacle);
		std::cerr << "Obstacle spawned off-screen at: " << obstacleX << ", " << obstacleY << std::endl;

		Obstacle* firstObstacle = nullptr;
		Obstacle* secondObstacle = nullptr;

		for(size_t i = 0; i < obstacles.size() - 1; ++i) {
			if (obstacles[i].getSprite().getPosition().x > playerX) {
				firstObstacle = &obstacles[i];
				secondObstacle = &obstacles[i + 1];
				break;
			}
		}
		if (firstObstacle && secondObstacle && enemies.empty()) {
			float enemyX = (firstObstacle->getSprite().getPosition().x + secondObstacle->getSprite().getPosition().x) / 2.f;
			float patrolStartX = firstObstacle->getSprite().getPosition().x + obstacleWidth;
			float patrolEndX = secondObstacle->getSprite().getPosition().x - obstacleWidth;

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
			return obstacle.getSprite().getPosition().x < playerX - cleanupThreshold;
		}), obstacles.end());

	enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
		[playerX, cleanupThreshold](const Werewolf& enemy) {
			return enemy.getShape().getPosition().x < playerX - cleanupThreshold && enemy.isAlive();
		}), enemies.end());
}

void GameManager::spawnBoss(float playerX) {
	if (!bossSpawned&& currentState == GameState::Playing) {
		const float bossWidth = 100.f;
		const float bossHeight = 200.f;
		sf::Vector2f bossPosition(playerX + 1000.f, ground.getPosition().y - bossHeight);

		Boss newBoss(bossPosition.x, bossPosition.y, bossWidth, bossHeight, 50.f, 0.f);
		bosses.push_back(newBoss);
		bossSpawned = true;
		std::cerr << "Boss spawned at position: " << bossPosition.x << ", " << bossPosition.y << std::endl;
	}
	else {
		std::cerr << "Boss already spawned. No new Boss created." << std::endl;
	}
}

void GameManager::renderBossHealth(sf::RenderWindow& window) {

	if (!bosses.empty()) {
		const Boss& boss = bosses[0];

		sf::RectangleShape healthBar;
		healthBar.setSize(sf::Vector2f(boss.getHealth() * 2, 20.f));
		healthBar.setFillColor(sf::Color::Red);

		const sf::Vector2f viewCenter = window.getView().getCenter();
		const sf::Vector2f viewSize = window.getView().getSize();
		float healthBarX = viewCenter.x - (viewSize.x / 2) + 20.f;
		float healthBarY = viewCenter.y - (viewSize.y / 2) + 20.f;
		healthBar.setPosition(healthBarX, healthBarY);
		window.draw(healthBar);

	}
}

void GameManager::updateGround(float playerX) {
	const float extensionThreshold = 1500.f;

	if (playerX > ground.getPosition().x + ground.getSize().x - extensionThreshold) {
		ground.setSize(sf::Vector2f(ground.getSize().x + extensionThreshold, ground.getSize().y));
	}

	if (playerX < ground.getPosition().x + extensionThreshold) {
		ground.setPosition(ground.getPosition().x - extensionThreshold, ground.getPosition().y);
		ground.setSize(sf::Vector2f(ground.getSize().x + extensionThreshold, ground.getSize().y));
	}
	ground.setPosition(ground.getPosition().x, static_cast<float>(window.getSize().y) - ground.getSize().y);
}

void GameManager::render() {
	window.clear();
	if (currentState == GameState::Playing) {
		window.setView(camera);

		window.draw(ground);
		window.draw(player.getShape());

		sf::FloatRect cameraBounds(camera.getCenter().x - camera.getSize().x / 2,
			camera.getCenter().y - camera.getSize().y / 2,
			camera.getSize().x,
			camera.getSize().y);

		for (const auto& obstacle : obstacles) {
			if (cameraBounds.intersects(obstacle.getSprite().getGlobalBounds())) {
				obstacle.render(window); // Render only obstacles visible in the camera's view
			}
		}
			for (const auto& enemy : enemies) {
				window.draw(enemy.getShape());
			}
			for (const auto& projectile : projectiles) {
				projectile.render(window);
			}
			for (const auto& boss : bosses) {
				window.draw(boss.getShape());
			}

			renderBossHealth(window);
			float timeElapsed = gameClock.getElapsedTime().asSeconds();
			inGameUI->update(score, timeElapsed, player.getHealth());
			inGameUI->render(window);
	}
	else if (currentState == GameState::WinScreen) {
		window.clear();
		if (winScreenUI) {
			winScreenUI->render(window);
			std::cerr << "Rendering Win Screen ..." << std::endl;
		}
		else {
			std::cerr << "Win Screen UI is not initialized!" << std::endl;
		}

	}
	else if (currentState == GameState::GameOver) {
		window.clear();

		if (gameOverUI) {
			gameOverUI->display(window, finalScore, finalTime);
		}
	}
	window.display();
}

void GameManager::checkCollisions() {
	for (auto it = projectiles.begin(); it != projectiles.end();) {
		const sf::FloatRect projectileBounds = it->getSprite().getGlobalBounds();
		
		bool collisionDetected = false;
		for (const auto& obstacle : obstacles) {
			if (projectileBounds.intersects(obstacle.getSprite().getGlobalBounds())) {
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
		const sf::FloatRect obstacleBounds = obstacle.getSprite().getGlobalBounds();

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
			float damageAmount = enemy.getIsBoss() ? 15.f : 5.f;
			player.takeDamage(damageAmount);
			score -= enemy.getIsBoss() ? 20 : 10;;

			const sf::Vector2f playerPosition = player.getShape().getPosition();
			const sf::Vector2f enemyPosition = enemy.getShape().getPosition();

			if (playerPosition.x < enemyPosition.x) {
				player.getShape().setPosition(playerPosition.x - 50.f, playerPosition.y);
			}
			else {
				player.getShape().setPosition(playerPosition.x + 50.f, playerPosition.y);
			}
			if (player.getHealth() <= 0) {
				gameOver();
				return;
			}
		}
	}

	for (auto& enemy : enemies) {
		for (auto it = projectiles.begin(); it != projectiles.end();) {
			if (it->getSprite().getGlobalBounds().intersects(enemy.getShape().getGlobalBounds())) {
				float damageAmount = 5.f;
				enemy.takeDamage(damageAmount);
				score += enemy.getIsBoss() ? 30 : 15;
				if (!enemy.isAlive()) {
					score += enemy.getIsBoss() ? 100 : 50;
				}
				it = projectiles.erase(it);
			}
			else {
				++it;
			}
		}
	}

	for (auto& boss : bosses) {
		for (auto it = projectiles.begin(); it != projectiles.end();) {
			if (it->getSprite().getGlobalBounds().intersects(boss.getShape().getGlobalBounds())) {
				boss.takeDamage(5.f);
				score += boss.isAlive() ? 30 : 100;

				it = projectiles.erase(it);
			}
			else {
				++it;
			}
		}
	}

	for (auto& boss : bosses) {
		if (player.getShape().getGlobalBounds().intersects(boss.getShape().getGlobalBounds())) {
			player.takeDamage(15.f);

			sf::FloatRect playerBounds = player.getShape().getGlobalBounds();
			sf::FloatRect bossBounds = boss.getShape().getGlobalBounds();

			if (playerBounds.left + playerBounds.width > bossBounds.left && playerBounds.left < bossBounds.left){
				player.getShape().setPosition(bossBounds.left - playerBounds.width, playerBounds.top);
			}
			else if (playerBounds.left < bossBounds.left + bossBounds.width && playerBounds.left + playerBounds.width > bossBounds.left + bossBounds.width){
				player.getShape().setPosition(bossBounds.left + bossBounds.width, playerBounds.top);
			}

			if (player.getHealth() <= 0) {
				gameOver();
				return;
			}
		}
	}

	enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const Werewolf& enemy) {
		return !enemy.isAlive();
	}), enemies.end());

	bosses.erase(std::remove_if(bosses.begin(), bosses.end(), [](const Boss& boss) {
		return !boss.isAlive();
	}), bosses.end());

	if (bosses.empty()) {
		bossSpawned = false;
	}

	for (auto& boss : bosses){

		if (!boss.isAlive()) {
			currentState = GameState::WinScreen;
			finalScore = score;
			finalTime = gameClock.getElapsedTime().asSeconds();

			winScreenUI->update(finalScore, finalTime);
			bosses.clear();
			std::cerr << " Boss defeated! Transitioning to Win Screen..." << std::endl;
			return;
		}
	}
}

void GameManager::resetGame() {
	score = 0;
	finalScore = 0;
	finalTime = 0.f;
	bossSpawned = false;
	player.reset();
	gameClock.restart();

	obstacles.clear();
	enemies.clear();
	projectiles.clear();
	bosses.clear();

	initializeEntities(player.getShape().getPosition().x);
	spawnEntities(player.getShape().getPosition().x);
}
