#pragma once

namespace Breakout {

class GameTime {
public:
    static float deltaTime() { return dt; }
    static float realtimeSinceStartup() { return rt;}

    static void update() {
        auto now = std::chrono::steady_clock::now();
        dt = std::chrono::duration<float>(now - lastFrame).count();
        rt = std::chrono::duration<float>(now - startTime).count();
        lastFrame = now;
    }

private:
    static inline std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
    static inline std::chrono::steady_clock::time_point lastFrame = startTime;
    static inline float dt = 0.0f;
    static inline float rt = 0.0f;
};

}