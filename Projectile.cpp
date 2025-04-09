#include "Projectile.h"

Projectile::Projectile(float startX, float startY, const sf::Texture& texture){
	sprite.setTexture(texture);
	sprite.setPosition(startX, startX);
	velocity = sf::Vector2f(500.f, 0.f);
}

void Projectile::update(float deltaTime) {
	sprite.move(velocity * deltaTime);
}

void Projectile::render(sf::RenderWindow& window) const {
	window.draw(sprite);
}

const sf::Sprite& Projectile::getSprite() const {
	return sprite;
}
