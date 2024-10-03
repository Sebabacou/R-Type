/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "registry.hpp"
#include "create_ennemies.hpp"
#include "system.hpp"
#include "game_state_manager.hpp"
#include "FPSCounter.hpp"

namespace rtype {
    class Game {
        public:
            Game(const std::string& title, unsigned int width, unsigned int height, const std::string& playerTexturePath);
            void run();
        private:
            void initChargeBullet();
            void processEvents(GameStateManager& gameStateManager);
            void update();
            void render();
            void renderMenu();
            void renderPauseScreen();
            void renderGameOverScreen();
            void renderGame();
            void startGame();
            void createMenuButtons(sf::Font& font);

            ecs::CreateEnnemies createEnnemies;
            sf::RenderWindow window;
            ecs::Registry registry;
            ecs::System system;
            sf::Texture playerTexture;
            sf::Sprite playerSprite;
            GameStateManager gameStateManager;

            // FPS Counter
            sf::Text fpsText;
            FPSCounter fpsCounter;
    };
}
