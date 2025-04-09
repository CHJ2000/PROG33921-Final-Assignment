#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SFML/Graphics.hpp>


class Obstacle {
public: 
	Obstacle(float x, float y, float width, float height, const sf::Texture& texture);
	const sf::Sprite& getSprite() const;
	void render(sf::RenderWindow& window) const;

private: 
	sf::Sprite sprite;

};

#endif 