#pragma once

#include "IBrick.h"
#include "IBrickBehaviour.h"

namespace Breakout {

class MovingYArmorBehaviour : public IBrickBehavior {
public:
    static constexpr float BRICK_WIDTH = 160.0f;
    static constexpr float BRICK_HEIGHT = 60.0f;

    void hit(IBrick& brick) override {
        if (!brick.getIsDamaged()) {
            brick.damage();
        } else {
            brick.destroy();
        }
    }

    virtual void update(IBrick& brick) {
        float amplitude = 40.0f;
        float speed = 1.5f;

        float t = GameTime::realtimeSinceStartup();
        float offsetY = amplitude * std::sin(speed * t);

        Rect bounds;
        bounds.x = brick.getGridX() * BRICK_WIDTH;
        bounds.y = brick.getGridY() * BRICK_HEIGHT + offsetY;
        bounds.w = BRICK_WIDTH;
        bounds.h = BRICK_HEIGHT;

        brick.setBounds((bounds));
    }
};

}