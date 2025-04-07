#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SFML/Graphics.hpp>
#include "Wizard.h"
#include "Werewolf.h"
#include "Projectile.h"
#include "MainMenu.h"
#include "InGameUI.h"
#include "GameOverMenu.h"
#include "Obstacle.h"


class GameManager {
public:
	GameManager();
	void run();

private:
	void mainMenu();
	void handleMainMenuEvents();
	void renderMainMenu();
	void gamePlay();
	void gameOver();
	void handleGameOverEvents();
	void renderGameOver();
	void handleEvents();
	void update(float deltaTime);
	void render();
	void checkCollisions();
	void initializeEntities(float playerX);
	void spawnEntities(float playerX);
	void cleanUpEntities(float playerX);
	void updateGround(float playerX);
	void resetGame();
	enum class GameState {Playing, GameOver, MainMenu};
	GameState currentState = GameState::Playing;

	sf::RenderWindow window;
	sf::Font font;
	sf::View camera;
	Wizard player;
	std::vector<Werewolf> enemies;
	std::vector<Projectile> projectiles;
	std::vector<Obstacle> obstacles;
	sf::RectangleShape ground;


	MainMenu* mainMenuUI;
	InGameUI* inGameUI;
	GameOverMenu* gameOverUI;

	int score;
	sf::Clock gameClock;
	float finalTime = 0;

};
#endif
