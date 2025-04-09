#include "Boss.h"
#include <SFML/Graphics.hpp>
#include "GameManager.h"
#include "Obstacle.h"


Boss::Boss(float x, float y, float width, float height, float health, float attackDamage) 
	: health(health), attackDamage(attackDamage), patrolStart(x - 100.f, y), patrolEnd(x + 100.f, y), isBoss(true){
	shape.setSize(sf::Vector2f(width, height));
	shape.setFillColor(sf::Color::White);
	shape.setPosition(x, y);
}

void Boss::move(float deltaTime, const sf::Vector2f& playerPosition, const std::vector<Obstacle>& obstacles) {
	sf::Vector2f direction;
	float length;

	float range = 300.f;

	sf::Vector2f playerDirection = playerPosition - shape.getPosition();
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
		const sf::FloatRect bossBounds = shape.getGlobalBounds();
		const sf::FloatRect obstacleBounds = obstacle.getShape().getGlobalBounds();
		if (bossBounds.intersects(obstacleBounds)) {
			if (direction.x > 0.f && bossBounds.left + bossBounds.width > obstacleBounds.left) {
				shape.setPosition(obstacleBounds.left - bossBounds.width, bossBounds.top);
			}
			if (direction.x < 0.f && bossBounds.left < obstacleBounds.left + obstacleBounds.width) {
				shape.setPosition(obstacleBounds.left + obstacleBounds.width, bossBounds.top);
			}
		}
	}

}

void Boss::takeDamage(float damage) {
	health -= damage;
	if (health < 0) {
		health = 0;
	}
}

bool Boss::isAlive() const {
	return health > 0;
}

float Boss::getHealth() const {
	return health;
}

bool Boss::isDefeated() const {
	return health <= 0.f;
}

bool Boss::getIsBoss() const {
	return isBoss;
}

sf::RectangleShape Boss::getShape() const {
	return shape;
}