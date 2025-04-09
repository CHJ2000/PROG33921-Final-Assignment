#include "Obstacle.h"

Obstacle::Obstacle(float x, float y, float width, float height, const sf::Texture& texture) {
	sprite.setTexture(texture);
	sprite.setScale(width / texture.getSize().x, height / texture.getSize().y);;
	sprite.setPosition(x, y);
}

void Obstacle::render(sf::RenderWindow& window) const {
	window.draw(sprite);
}

const sf::Sprite& Obstacle::getSprite() const {
	return sprite;
}
