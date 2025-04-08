#ifndef WEREWOLF_H

#define WEREWOLF_H

#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>
#include "Obstacle.h"



class Werewolf {
public:
	Werewolf(float startX, float startY, float patrolEndX);
	void update(const sf::CircleShape& player, float deltaTime, const std::vector<Obstacle>& obstacles);
	const sf::RectangleShape& getShape() const;
	void takeDamage(float damage);
	bool isAlive() const;
	bool getIsBoss() const;
	bool isBoss = false;
private:
	sf::RectangleShape shape;
	sf::Vector2f patrolStart;
	sf::Vector2f patrolEnd;
	bool movingToEnd;
	int health = 20;
	
};

#endif

