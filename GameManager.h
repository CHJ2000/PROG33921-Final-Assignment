#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SFML/Graphics.hpp>
#include "Wizard.h"
#include "Werewolf.h"
#include "Projectile.h"
#include "MainMenu.h"
#include "InGameUI.h"
#include "GameOverMenu.h"


class GameManager {
public:
	GameManager();
	void run();

private:
	void mainMenu();
	void gameplay();
	void gameOver();
	void handleEvents();
	void update(float deltaTime);
	void render();
	void checkCollisions();

	sf::RenderWindow window;
	sf::Font font;
	sf::View camera;
	Wizard player;
	std::vector<Werewolf> enemies;
	std::vector<Projectile> projectiles;
	sf::RectangleShape ground;
	sf::RectangleShape obstacle1;
	sf::RectangleShape obstacle2;

	MainMenu* mainMenuUI;
	InGameUI* inGameUI;
	GameOverMenu* gameOverUI;

	int score;
	sf::Clock gameClock;

};
#endif
