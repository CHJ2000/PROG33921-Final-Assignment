#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SFML/Graphics.hpp>


class Obstacle {
public: 
	Obstacle(float x, float y, float width, float height);
	const sf::RectangleShape& getShape() const;
	void update();

private: 
	sf::RectangleShape shape;

};

#endif 