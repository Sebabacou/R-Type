/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include "game/Game.hpp"

int main() {
    rtype::Game game("R-Type", 2560, 1080);
    game.run();
    return 0;
}