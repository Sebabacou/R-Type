#if __cpp_lib_modules < 202207L
module;

#include <chrono>
#endif
export module game:components.projectiles;

#if __cpp_lib_modules >= 202207L
import std;
#endif
import ecs;

export namespace game::components {
    struct projectile {
        int damage;
        std::chrono::steady_clock::time_point birth;
        std::chrono::steady_clock::duration lifetime;
    };

    struct projectile_launcher {
        const std::chrono::steady_clock::duration cooldown;
        std::chrono::steady_clock::time_point last_shot;
        ecs::entity game;
    };
}