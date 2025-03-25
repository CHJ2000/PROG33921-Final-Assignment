#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>

class Projectile {
public:
	Projectile(float startX, float startY, float speed);
	void update(float deltaTime);
	const sf::CircleShape& getShape() const;

private:
	sf::CircleShape shape;
	float speed;
};

#endif


