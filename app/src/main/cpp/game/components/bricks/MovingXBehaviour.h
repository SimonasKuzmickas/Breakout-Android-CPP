#pragma once

#include "IBrick.h"
#include "IBrickBehaviour.h"

namespace Breakout {

class MovingXBehaviour : public IBrickBehavior {
public:
    static constexpr float BRICK_WIDTH = 160.0f;
    static constexpr float BRICK_HEIGHT = 60.0f;

    virtual void update(IBrick& brick) {
        float amplitude = 80.0f;
        float speed = 2.0f;

        float t = GameTime::realtimeSinceStartup();
        float offsetX = amplitude * std::sin(speed * t);

        Rect bounds;
        bounds.x = brick.getGridX() * BRICK_WIDTH + offsetX;
        bounds.y = brick.getGridY() * BRICK_HEIGHT;
        bounds.w = BRICK_WIDTH;
        bounds.h = BRICK_HEIGHT;

        brick.setBounds((bounds));
    }

    void hit(IBrick& brick) override {
        brick.destroy();
    }
};

}