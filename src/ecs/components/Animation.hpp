/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Animation
*/

#pragma once

namespace ecs {
        struct Animation {
        sf::Texture texture;
        sf::Sprite sprite;
        sf::Vector2i imageCount;
        sf::Vector2i currentImage;
        sf::Vector2i imageSize;
        float switchTime;
        float totalTime;

        Animation(const std::string& spriteSheetPath = nullptr, int imageCountX = 0, int imageCountY = 0, float switchTime = 0.0f)
            : imageCount(imageCountX, imageCountY), currentImage(0, 0), switchTime(switchTime), totalTime(0.0f) {
            if (!texture.loadFromFile(spriteSheetPath)) {
                throw std::runtime_error("Failed to load sprite sheet");
            }
            sprite.setTexture(texture);
            imageSize = sf::Vector2i(texture.getSize().x / imageCount.x, texture.getSize().y / imageCount.y);
            sprite.setTextureRect(sf::IntRect(0, 0, imageSize.x, imageSize.y));
        }
    };
}
