#include "Projectile.h"

Projectile::Projectile(float startX, float startY, float speed)
	:speed(speed) {
	shape.setRadius(5.f);
	shape.setFillColor(sf::Color::Yellow);
	shape.setPosition(startX, startY);
}

void Projectile::update(float deltaTime) {
	shape.move(speed * deltaTime, 0.f);
}

const sf::CircleShape& Projectile::getShape() const {
	return shape;
}
