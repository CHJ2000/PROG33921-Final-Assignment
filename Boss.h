#ifndef BOSS_H
#define BOSS_H

#include <SFML/Graphics.hpp>
#include "Wizard.h"
#include "Obstacle.h"
class Boss {
public:
	Boss(float x, float y, float width, float heigth, float health, float attackDamage);
	void move(float deltaTime, const sf::Vector2f& playerPosition, const std::vector<Obstacle>& obstacles);
	void takeDamage(float damage);
	bool isAlive() const;
	float getHealth() const;
	bool isDefeated() const;
	bool getIsBoss() const;
	sf::RectangleShape getShape() const;

private:
	sf::RectangleShape shape;
	int health = 50;
	float attackDamage;
	sf::Vector2f patrolStart;
	sf::Vector2f patrolEnd;
	bool isBoss = true;
	bool movingToEnd = false;
};

#endif