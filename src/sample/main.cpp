#include <csignal>
#if __cpp_lib_modules < 202207L
#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#endif

#include <SFML/Graphics.hpp>

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;
import utils;
import game;

using namespace ecs;
using namespace game::components;

using namespace std::chrono_literals;

void spawn_enemy(const ecs::components::position& position, game::game &game)
{
    const ecs::components::gui::asset_manager &asset_manager = *game.get_entity_component<const ecs::components::gui::asset_manager>(game);
    auto e = game.create_entity();
    game.add_component(e, enemy{1, 1, std::chrono::steady_clock::now()});
    game.add_component(e, ecs::components::position{position.x, position.y});
    game.add_component(e, ecs::components::engine::velocity{10.0f, 0.0f});
    game.add_component(e, enemy_loop_movement{0.0f, 2000.0f, 200.0f, 800.0f, 1.0f, 0.0f, 100.0f, 2.0f});
    game.emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{game,
        std::container<ecs::components::gui::drawable::elements_container>::make({
            {static_cast<ecs::entity>(game), std::make_unique<ecs::components::gui::display_element>(
                std::make_unique<sf::Text>("Enemy", asset_manager.get_font("arial"), 12), "arial")},
            {static_cast<ecs::entity>(game), std::make_unique<ecs::components::gui::animation>(
                asset_manager.get_texture("enemy"), 1, 8, 200ms, "enemy")
            }
        })
    });
}

static void spawn_enemy_chaser(ecs::entity target, const ecs::components::position& position, game::game &game)
{
    const ecs::components::gui::asset_manager &asset_manager = *game.get_entity_component<const ecs::components::gui::asset_manager>(game);
    auto e = game.create_entity();
    game.add_component(e, enemy{100, 10, std::chrono::steady_clock::now()});
    game.add_component(e, ecs::components::position{position.x, position.y});
    game.add_component(e, ecs::components::engine::velocity{10.0f, 0.0f});
    game.add_component(e, enemy_chaser{target, 10.0f});
    game.emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{game,
        std::container<ecs::components::gui::drawable::elements_container>::make({
            {static_cast<ecs::entity>(game), std::make_unique<ecs::components::gui::display_element>(
                std::make_unique<sf::Text>("Chaser", asset_manager.get_font("arial"), 12), "arial")},
            {static_cast<ecs::entity>(game), std::make_unique<ecs::components::gui::animation>
                (asset_manager.get_texture("enemy_chaser"), 1, 3, 200ms, "enemy_chaser")
            }
        })
    });
}

static void spawn_enemy_spawner(const ecs::components::position& position, game::game &game)
{
    const auto now = std::chrono::steady_clock::now();
    const ecs::components::gui::asset_manager &asset_manager = *game.get_entity_component<const ecs::components::gui::asset_manager>(game);
    auto e = game.create_entity();
    game.emplace_component<enemy>(e, 100, 10, now);
    game.emplace_component<enemy_spawner>(e, 2s, 5ul, game);
    game.add_component(e, position);
    game.emplace_component<ecs::components::engine::velocity>(e);
    game.emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{game,
        std::container<ecs::components::gui::drawable::elements_container>::make({
            {static_cast<ecs::entity>(game), std::make_unique<ecs::components::gui::display_element>(
                std::make_unique<sf::Text>("Spawner", asset_manager.get_font("arial"), 12), "arial")},
            {static_cast<ecs::entity>(game), std::make_unique<ecs::components::gui::animation>
                (asset_manager.get_texture("enemy_spawner"), 1, 5, 200ms, "enemy_spawner")
            }
        })
    });
}

static void spawn_enemy_shooter(const ecs::components::position& position, game::game &game)
{
    const ecs::components::gui::asset_manager &asset_manager = *game.get_entity_component<const ecs::components::gui::asset_manager>(game);
    auto e = game.create_entity();
    game.add_component(e, enemy{100, 10, std::chrono::steady_clock::now()});
    game.add_component(e, ecs::components::position{position.x, position.y});
    game.add_component(e, ecs::components::engine::velocity{10.0f, 0.0f});
    game.add_component(e, enemy_shooter{2s, game});
    game.emplace_component<ecs::components::gui::drawable>(e, ecs::components::gui::drawable{game,
        std::container<ecs::components::gui::drawable::elements_container>::make({
            {static_cast<ecs::entity>(game), std::make_unique<ecs::components::gui::display_element>(
                std::make_unique<sf::Text>("Shooter", asset_manager.get_font("arial"), 12), "arial")},
            {static_cast<ecs::entity>(game), std::make_unique<ecs::components::gui::animation>
                (asset_manager.get_texture("enemy_shooter"), 1, 3, 200ms, "enemy_shooter")
            }
        })
    });
}


static void initializeEnemies(game::game &game) noexcept {
    spawn_enemy({100.0f, 100.0f}, game);
    spawn_enemy_chaser(game.player, {200.0f, 200.0f}, game);
    spawn_enemy_spawner({300.0f, 300.0f}, game);
    spawn_enemy_shooter({400.0f, 400.0f}, game);
}

int main()
{
    game::game game;
    initializeEnemies(game);
    auto button = game.create_entity();

    const ecs::components::gui::asset_manager &asset_manager = *game.get_entity_component<const ecs::components::gui::asset_manager>(game);
    projectile_launcher &launcher = *game.get_entity_component<projectile_launcher>(game.player);
    game.emplace_component<components::position>(button, 230.f, 875.f);
    game.emplace_component<game::components::button>(button, 100.f, 50.f, "Fire", asset_manager.get_font("arial"),
            [&launcher](){ launcher.last_shot = std::chrono::steady_clock::time_point(0s); }, game);

    game.run();

    return 0;
}
