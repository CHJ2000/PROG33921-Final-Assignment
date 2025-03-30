#include "Werewolf.h"
#include <cmath>

Werewolf::Werewolf(float startX, float startY, float patrolEndX)
: patrolStart(startX, startY), patrolEnd(patrolEndX, startY), movingToEnd(true), health(3) {
	shape.setSize({ 40.f, 40.f });
	shape.setFillColor(sf::Color::Red);
	shape.setPosition(startX, startY);
}

void Werewolf::update(const sf::CircleShape& player, float deltaTime, const sf::RectangleShape& obstacle1) {
	sf::Vector2f direction;
	float length;

	//const float groundY = patrolStart.y;
	float range = 300.f;

	sf::Vector2f playerDirection = player.getPosition() - shape.getPosition();
	float playerDistance = std::sqrt(playerDirection.x * playerDirection.x + playerDirection.y * playerDirection.y);

	if (playerDistance < range) {
		direction = playerDirection;
		direction.y = 0.f;
		length = std::sqrt(direction.x * direction.x);
		if (length != 0) direction /= length;
		float speed = 150.f;
		shape.move(direction * speed * deltaTime);
	}
	else {
		direction = movingToEnd ? (patrolEnd - shape.getPosition()) : (patrolStart - shape.getPosition());
		direction.y = 0.f;
		length = std::sqrt(direction.x * direction.x);
		if (length != 0) direction /= length;
		float speed = 150.f;
		shape.move(direction * speed * deltaTime);

		if (length < 1.f) movingToEnd = !movingToEnd;
	}

	const sf::FloatRect enemyBounds = shape.getGlobalBounds();
	const sf::FloatRect obstacle1Bounds = obstacle1.getGlobalBounds();

	if (enemyBounds.intersects(obstacle1Bounds)) {
		if (direction.x > 0.f && enemyBounds.left + enemyBounds.width > obstacle1Bounds.left) {
			shape.setPosition(obstacle1Bounds.left - enemyBounds.width, enemyBounds.top);
		}
		if (direction.x < 0.f && enemyBounds.left < obstacle1Bounds.left + obstacle1Bounds.width) {
			shape.setPosition(obstacle1Bounds.left + obstacle1Bounds.width, enemyBounds.top);
		}
	}
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
