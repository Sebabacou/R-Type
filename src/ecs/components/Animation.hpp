/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Animation
*/

#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

namespace ecs {
    /**
     * @brief Class that contains the animation of an entity
     *
     * @param sprite Sprite of the entity
     * @param imageCount Number of images in the sprite
     * @param currentImage Current image of the sprite
     * @param imageSize Size of the image
     * @param switchTime Time between two images
     * @param totalTime Total time of the animation
     */
    struct Animation {
        sf::Sprite sprite;
        sf::Vector2i imageCount;
        sf::Vector2i currentImage;
        sf::Vector2i imageSize;
        float switchTime;
        float totalTime;

        /**
         * @brief Construct a new Animation object
         *
         * @param spriteSheetPath Path to the sprite sheet
         * @param imageCountX Number of images on the x axis
         * @param imageCountY Number of images on the y axis
         * @param switchTime Time between two images
         */
        Animation(const std::string& spriteSheetPath = "", int imageCountX = 0, int imageCountY = 0, float switchTime = 0.0f)
            : imageCount(imageCountX, imageCountY), currentImage(0, 0), switchTime(switchTime), totalTime(0.0f) {
            static sf::Texture sharedTexture;
            if (!sharedTexture.loadFromFile(spriteSheetPath)) {
                throw std::runtime_error("Failed to load sprite sheet");
            } else {
                std::cout << "Loaded sprite sheet" << std::endl;
            }

            imageSize = sf::Vector2i(sharedTexture.getSize().x / imageCount.x, sharedTexture.getSize().y / imageCount.y);

            sprite.setTexture(sharedTexture);
        }

        /**
         * @brief Update the animation
         *
         * @param deltaTime Time between two frames
         */
        void update(float deltaTime) {
            totalTime += deltaTime;

            if (totalTime >= switchTime) {
                totalTime -= switchTime;

                currentImage.x++;
                if (currentImage.x >= imageCount.x) {
                    currentImage.x = 0;
                    currentImage.y++;

                    if (currentImage.y >= imageCount.y) {
                        currentImage.y = 0;
                    }
                }
                sprite.setTextureRect(sf::IntRect(currentImage.x * imageSize.x,
                                                currentImage.y * imageSize.y,
                                                imageSize.x,
                                                imageSize.y));
            }
        }
    };
}
