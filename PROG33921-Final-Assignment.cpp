// PROG33921-Final-Assignment.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <SFML/Graphics.hpp>
#include "Wizard.h"
#include "Werewolf.h"
#include <vector>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Wizards n Werevolves");
    window.setFramerateLimit(60);

    Wizard player(100.f, 500.f);

    Werewolf enemy(600.f, 500.f);

    std::vector<sf::CircleShape> projectiles;

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                player.attack(projectiles);
            }
        }

        sf::Vector2f movement(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            movement.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            movement.x += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            player.jump();

        player.move(movement, deltaTime.asSeconds(), 200.f);
        player.update(deltaTime.asSeconds());
        enemy.update(player.getShape(), deltaTime.asSeconds());

        for (auto& projectile : projectiles) {
            projectile.move(300.f * deltaTime.asSeconds(), 0.f);
        }

        window.clear();
        window.draw(player.getShape());
        for (const auto& projectile : projectiles) {
            window.draw(projectile);
        }
        window.display();
    }
    return 0;
}