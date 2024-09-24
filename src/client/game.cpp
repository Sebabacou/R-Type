/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game
*/

#include "game.hpp"
#include <iostream>

namespace rtype {
    Game::Game(const std::string& title, unsigned int width, unsigned int height, const std::string& playerTexturePath)
        : window(sf::VideoMode(width, height), title) {
        window.setFramerateLimit(60);
    }

    void Game::run() {
        registry.register_all_components();

        auto movable_entity = registry.spawn_entity();
        registry.emplace_component<ecs::Position>(movable_entity, 400.0f, 300.0f);
        registry.emplace_component<ecs::Velocity>(movable_entity, 0.0f, 0.0f);
        registry.emplace_component<ecs::Drawable>(movable_entity, "assets/Ship/Ship.png");
        registry.emplace_component<ecs::Controllable>(movable_entity, true, 5.0f);
        registry.emplace_component<ecs::Acceleration>(movable_entity, 0.0f, 0.0f);
        registry.emplace_component<ecs::Collision>(movable_entity, 0.0f, false, sf::Rect<float>(0.0f, 0.0f, 50.0f, 50.0f));

        while (window.isOpen()) {
            processEvents();
            update();
            render();
        }
    }

    void Game::processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }
    }

    void Game::update() {
        system.control_system(registry);
        system.position_system(registry);
        system.loop_movement_system(registry);

        auto& positions = registry.get_components<ecs::Position>();
        auto& drawables = registry.get_components<ecs::Drawable>();

        for (std::size_t i = 0; i < positions.size(); ++i) {
            if (positions[i] && drawables[i]) { // Ensure both components exist
                // Boundary checks for positions
                if (positions[i]->x < 0) positions[i]->x = 0;
                if (positions[i]->x > window.getSize().x - drawables[i]->size) // Use size from Drawable
                    positions[i]->x = window.getSize().x - drawables[i]->size;
                if (positions[i]->y < 0) positions[i]->y = 0;
                if (positions[i]->y > window.getSize().y - drawables[i]->size)
                    positions[i]->y = window.getSize().y - drawables[i]->size;
            }
        }
    }


    void Game::render() {
        window.clear();
        system.draw_system(registry, window); // Update to just pass the registry and window
        window.display();
    }
}
