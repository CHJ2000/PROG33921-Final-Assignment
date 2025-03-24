#ifndef WIZARD_H
#define WIZARD_H

#include <SFML/Graphics.hpp>

class Wizard {

public:
	Wizard(float startX, float startY);
	void move(sf::Vector2f direction, float deltaTime, float speed);
	void jump();
	void update(float deltaTime);
	void attack(std::vector<sf::CircleShape>& projectiles);
	void takeDamage();
	sf::CircleShape& getShape();
	int getHealth() const;

private:
	sf::CircleShape shape;
	sf::Vector2f velocity;
	bool isJumping;
	const float gravity = 500.f;
	int health;
};

#endif 