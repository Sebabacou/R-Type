/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Game
*/

#include "Game.hpp"
#include "../menu/MainMenuState.hpp"
#include "GamePlayState.hpp"

namespace rtype {

    Game::Game(const std::string& title, unsigned int width, unsigned int height)
        : window(sf::VideoMode(width, height), title) {
        window.setFramerateLimit(60);
        currentState = std::make_unique<MainMenuState>(window, *this);
        if (!backgroundShader.loadFromFile("assets/shaders/background.frag", sf::Shader::Fragment)) {
            throw std::runtime_error("Could not load shader");
        }
    }

    void Game::run() {
        while (window.isOpen()) {
            currentState->handleInput();
            currentState->update();
            currentState->render();
        }
    }

    void Game::changeState(std::unique_ptr<GameState> newState) {
        currentState = std::move(newState);
    }

}
