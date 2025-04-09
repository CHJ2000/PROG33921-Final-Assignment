#ifndef WIZARD_H
#define WIZARD_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Projectile.h"


class Wizard {

public:
	Wizard(float startX, float startY);
	void move(sf::Vector2f direction, float deltaTime, float speed);
	void jump();
	void update(float deltaTime);
	void attack(std::vector<Projectile>& projectiles);
	void takeDamage(float damage);
	void stopFalling();
	void stopMovingHorizontally();
	sf::CircleShape& getShape();
	sf::Vector2f getVelocity() const;
	int getHealth() const;
	bool isAlive() const;
	void reset();

private:
	sf::CircleShape shape;
	sf::Vector2f velocity;
	bool isJumping;
	const float gravity = 500.f;
	int health = 50;
};

#endif 