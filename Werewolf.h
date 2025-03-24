#ifndef WEREWOLF_H

#define WEREWOLF_H

#include <SFML\Graphics.hpp>
#include <iostream>



class Werewolf {
public:
	Werewolf(float startX, float startY);
	void update(const sf::CircleShape& player, float deltaTime);
	sf::RectangleShape& getShape();
private:
	sf::RectangleShape shape;
};

#endif

