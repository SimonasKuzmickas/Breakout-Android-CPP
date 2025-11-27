#pragma once

namespace Breakout {

class GameTime {
public:
    static float deltaTime() { return dt; }

    static void update() {
        auto now = std::chrono::steady_clock::now();
        dt = std::chrono::duration<float>(now - lastFrame).count();
        lastFrame = now;
    }

private:
    static inline std::chrono::steady_clock::time_point lastFrame = std::chrono::steady_clock::now();
    static inline float dt = 0.0f;
};

}