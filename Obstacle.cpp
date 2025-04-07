#include "Obstacle.h"

Obstacle::Obstacle(float x, float y, float width, float height) {
	shape.setSize(sf::Vector2f(width, height));
	shape.setFillColor(sf::Color::Yellow);
	shape.setPosition(x, y);
}

const sf::RectangleShape& Obstacle::getShape() const {
	return shape;
}

void Obstacle::update() {

}