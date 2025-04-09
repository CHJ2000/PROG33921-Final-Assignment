#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>

class Projectile {
public:
	Projectile(float startX, float startY, const sf::Texture& texture);
	
	void update(float deltaTime);
	void render(sf::RenderWindow& window) const;
	const sf::Sprite& getSprite() const;

private:
	sf::Sprite sprite;
	sf::Vector2f velocity;
};

#endif


