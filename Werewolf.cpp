#include "Werewolf.h"
#include <cmath>
#include "Obstacle.h"

Werewolf::Werewolf(float startX, float startY, float patrolEndX)
: health(20), patrolStart(startX, startY), patrolEnd(patrolEndX, startY), movingToEnd(true), isBoss(false) {
	shape.setSize({ 40.f, 40.f });
	shape.setFillColor(sf::Color::Red);
	shape.setPosition(startX, startY);
}

void Werewolf::update(const sf::CircleShape& player, float deltaTime, const std::vector<Obstacle>& obstacles) {
	sf::Vector2f direction;
	float length;

	float range = 150.f;

	sf::Vector2f playerDirection = player.getPosition() - shape.getPosition();
	float playerDistance = std::sqrt(playerDirection.x * playerDirection.x + playerDirection.y * playerDirection.y);

	if (playerDistance < range) {
		direction = playerDirection;
		direction.y = 0.f;
		length = std::sqrt(direction.x * direction.x);
		if (length != 0) direction /= length;
		float speed = 75.f;
		shape.move(direction * speed * deltaTime);
	}
	else {
		direction = movingToEnd ? (patrolEnd - shape.getPosition()) : (patrolStart - shape.getPosition());
		direction.y = 0.f;
		length = std::sqrt(direction.x * direction.x);
		if (length != 0) direction /= length;
		float speed = 75.f;
		shape.move(direction * speed * deltaTime);

		if (length < 1.f) movingToEnd = !movingToEnd;
	}

	for (const auto& obstacle : obstacles) {
		const sf::FloatRect enemyBounds = shape.getGlobalBounds();
		const sf::FloatRect obstacleBounds = obstacle.getSprite().getGlobalBounds();
		if (enemyBounds.intersects(obstacleBounds)) {
			if (direction.x > 0.f && enemyBounds.left + enemyBounds.width > obstacleBounds.left) {
				shape.setPosition(obstacleBounds.left - enemyBounds.width, enemyBounds.top);
			}
			if (direction.x < 0.f && enemyBounds.left < obstacleBounds.left + obstacleBounds.width) {
				shape.setPosition(obstacleBounds.left + obstacleBounds.width, enemyBounds.top);
			}
		}
	}
}

const sf::RectangleShape& Werewolf::getShape() const {
	return shape;
}

void Werewolf::takeDamage(float damage) {
	health -= static_cast<int>(damage);
	if (health < 0) {
		health = 0;
	}
}

bool Werewolf::isAlive() const {
	return health > 0;
}

bool Werewolf::getIsBoss() const {
	return isBoss;
}
