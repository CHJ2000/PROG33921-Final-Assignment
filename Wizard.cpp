#include "Wizard.h"
#include <stdexcept>

Wizard::Wizard(float startX, float startY) : health(3), isJumping(false) {
	shape.setRadius(20.f);
	shape.setFillColor(sf::Color::Blue);
	shape.setPosition(startX, startY);
	velocity = { 0.f, 0.f };
}
void Wizard::move(sf::Vector2f direction, float deltaTime, float speed) {
	shape.move(direction * speed * deltaTime);
}

void Wizard::jump() {
	if (!isJumping) {
		velocity.y = -300.f;
		isJumping = true;
	}
}

void Wizard::update(float deltaTime) {
	velocity.y += gravity * deltaTime;
	shape.move(0.f, velocity.y * deltaTime);

	if (shape.getPosition().y > 500.f) {
		shape.setPosition(shape.getPosition().x, 500.f);
		velocity.y = 0.f;
		isJumping = false;
	}
}

void Wizard::attack(std::vector<sf::CircleShape>& projectiles) {
	sf::CircleShape projectile(5.f);
	projectile.setFillColor(sf::Color::Yellow);
	projectile.setPosition(shape.getPosition().x + shape.getRadius(), shape.getPosition().y + shape.getRadius());
	projectiles.push_back(projectile);
}

void Wizard::takeDamage() {
	health--;
}

sf::CircleShape& Wizard::getShape() {
	return shape;
}
int Wizard::getHealth() const {
	return health;
}


