#include "Werewolf.h"
#include <cmath>

Werewolf::Werewolf(float startX, float startY, float patrolEndX)
: patrolStart(startX, startY), patrolEnd(patrolEndX, startY), movingToEnd(true), health(3) {
	shape.setSize({ 40.f, 40.f });
	shape.setFillColor(sf::Color::Red);
	shape.setPosition(startX, startY);
}

void Werewolf::update(const sf::CircleShape& player, float deltaTime) {
	sf::Vector2f direction;
	float length;

	float groundY = patrolStart.y;
	float range = 300.f;

	sf::Vector2f playerDirection = player.getPosition() - shape.getPosition();
	float playerDistance = std::sqrt(playerDirection.x * playerDirection.x + playerDirection.y * playerDirection.y);

	if (playerDistance < range) {
		direction = playerDirection;
		length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		if (length != 0) direction /= length;
		float speed = 100.f;
		shape.move(direction * speed * deltaTime);
	}
	else {
		direction = movingToEnd ? (patrolEnd - shape.getPosition()) : (patrolStart - shape.getPosition());
		length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		if (length != 0) direction /= length;
		shape.move(direction * 100.f * deltaTime);

		if (length < 1.f) movingToEnd = !movingToEnd;
	}

	sf::Vector2f pos = shape.getPosition();
	pos.y = groundY;
	shape.setPosition(pos);
}

const sf::RectangleShape& Werewolf::getShape() const {
	return shape;
}

void Werewolf::takeDamage() {
	health--;
}

bool Werewolf::isAlive() const {
	return health > 0;
}
