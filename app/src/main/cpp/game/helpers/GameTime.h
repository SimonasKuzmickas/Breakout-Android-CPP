#pragma once

namespace Breakout {

class GameTime {
public:
    static float deltaTime() {
        return 1.0f / 60;
    }
};

}