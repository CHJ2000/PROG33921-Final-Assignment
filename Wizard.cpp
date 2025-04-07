#include "Wizard.h"
#include <stdexcept>
#include "Projectile.h"
#include <iostream>

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

void Wizard::attack(std::vector<Projectile>& projectiles) {
	Projectile newProjectile(
		shape.getPosition().x + shape.getRadius(),
		shape.getPosition().y + shape.getRadius(),
		300.f
	);
	projectiles.push_back(newProjectile);
}

void Wizard::takeDamage() {
	health--;
}

sf::CircleShape& Wizard::getShape() {
	return shape;
}

sf::Vector2f Wizard::getVelocity() const {
	return velocity;
}

int Wizard::getHealth() const {
	return health;
}

bool Wizard::isAlive() const {
	return health > 0;
}

void Wizard::stopFalling() {
	velocity.y = 0.f;
	isJumping = false;
}

void Wizard::stopMovingHorizontally() {
	velocity.x = 0.f;
}
void Wizard::reset() {
	health = 3;
	shape.setPosition(100.f, 500.f);
}


