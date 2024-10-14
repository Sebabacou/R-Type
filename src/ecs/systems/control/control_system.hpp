/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** control_system
*/

#pragma once

#include "registry.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <networkClient/Client.hpp>

namespace ecs::systems {
    class ControlSystem {
        /**
         * @brief Class that contains all the logic of the control system
         */
        public:
            /**
             * @brief Update the control system
             *
             * @param registry contains all the logic of the components
             */
            void update(Registry& registry, client::Client& network);

        private:
            float acceleration = 0.5f;
            float maxSpeed = 4.0f;
    };
}