#include "MainMenuState.hpp"
#include "../game/Game.hpp"
#include "../factories/button_factory.hpp"
#include <iostream>
#include "../game/GamePlayState.hpp"

namespace rtype {
    MainMenuState::MainMenuState(sf::RenderWindow& window, Game& game)
        : window(window), game(game), registry(game.getRegistry()), system(game.getSystem())  // Initialize registry and system here
    {
        if (!font.loadFromFile("assets/fonts/arial.ttf")) {
            throw std::runtime_error("Could not load font");
        }

        registry.register_all_components();

        createMenuButtons();
    }

    void MainMenuState::handleInput() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }
    }

    void MainMenuState::update() {
        system.button_system(registry, window);
    }

    void MainMenuState::render() {
        window.clear();

        system.draw_system(registry, window);
        system.button_system_render(registry, window);

        window.display();
    }

    void MainMenuState::startGame() {
        std::cout << "Starting the game..." << std::endl;
        game.changeState(std::make_unique<GamePlayState>(window));
    }

    void MainMenuState::createMenuButtons() {
        sf::Vector2u window_size = window.getSize();
        float button_height = 50.0f;
        float spacing = 20.0f;
        float total_height = (button_height + spacing) * 7;
        float start_y = (window_size.y - total_height) / 2.0f;

        auto createButton = [this, button_height](const std::string& label, float yPos, std::function<void()> callback) -> ecs::Entity {
            sf::Text text(label, font, 24);
            float button_width = text.getLocalBounds().width + 40.0f; // Add padding to the text width
            float button_x = (window.getSize().x - button_width) / 2.0f; // Center the button

            button_x += button_width / 2.0f;

            auto entity = registry.spawn_entity();
            registry.emplace_component<ecs::Button>(
                entity,
                ecs::ButtonFactory::create_button(
                    label,
                    sf::Vector2f(button_x, yPos),
                    sf::Vector2f(button_width, button_height),
                    font,
                    sf::Color::Blue,
                    sf::Color::Cyan,
                    sf::Color::Green,
                    sf::Color::White,
                    24,
                    callback
                )
            );
            return entity;
        };

        float yPos = start_y;
        createButton("Solo Campaign", yPos, [this]() { startGame(); });
        yPos += button_height + spacing;

        createButton("Multiplayer", yPos, [this]() { startGame(); });
        yPos += button_height + spacing;

        createButton("Customize Ship", yPos, [this]() { startGame(); });
        yPos += button_height + spacing;

        createButton("Settings", yPos, [this]() { startGame(); });
        yPos += button_height + spacing;

        createButton("Map Editor", yPos, [this]() { startGame(); });
        yPos += button_height + spacing;

        createButton("Game Options", yPos, [this]() { startGame(); });
        yPos += button_height + spacing;

        createButton("Quit", yPos, [this]() { window.close(); });
    }





}
