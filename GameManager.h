#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SFML/Graphics.hpp>
#include "Wizard.h"
#include "Werewolf.h"
#include "Projectile.h"
#include <vector>


class GameManager {
public:
	GameManager();
	void run();

private:
	void handleEvents();
	void update(float deltaTime);
	void render();
	void checkCollisions();

	sf::RenderWindow window;
	sf::View camera;
	Wizard player;
	std::vector<Werewolf> enemies;
	std::vector<Projectile> projectiles;
	sf::RectangleShape ground;
	sf::RectangleShape obstacle1;
	sf::RectangleShape obstacle2;

};
#endif
