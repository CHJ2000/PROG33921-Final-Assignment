#include "Werewolf.h"
#include <cmath>

Werewolf::Werewolf(float startX, float startY) {
	shape.setSize({ 40.f, 40.f });
	shape.setFillColor(sf::Color::Red);
	shape.setPosition(startX, startY);
}

void Werewolf::update(const sf::CircleShape& player, float deltaTime) {
	sf::Vector2f direction = player.getPosition() - shape.getPosition();
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length != 0) direction /= length;
	float speed = 100.f;
	shape.move(direction * speed * deltaTime);
}

sf::RectangleShape& Werewolf::getShape() {
	return shape;
}
