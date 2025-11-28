#pragma once

#include "../brick/IBrick.h"

namespace Breakout {

class MovingYArmorBehaviour : public IBrick {
public:

    static constexpr float BRICK_WIDTH = 160.0f;
    static constexpr float BRICK_HEIGHT = 60.0f;

    MovingYArmorBehaviour(int gx, int gy)
            : IBrick(gx, gy, IBrick::BrickType::DynamicGreen) {
        isDynamic = true;
    }

    void hit() override {
        if (!isDamaged) {
            isDamaged = true;
        } else {
            isDestroyed = true;
        }
    }

    void update() override {
        float amplitude = 40.0f;
        float speed = 1.5f;

        float t = GameTime::realtimeSinceStartup();
        float offsetY = amplitude * std::sin(speed * t);

        bounds.x = gridX * BRICK_WIDTH;
        bounds.y = gridY * BRICK_HEIGHT + offsetY;
        bounds.w = BRICK_WIDTH;
        bounds.h = BRICK_HEIGHT;
    }
};

}