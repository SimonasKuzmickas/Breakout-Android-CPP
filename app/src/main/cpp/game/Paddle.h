#pragma once
#include "Math.h"
#include <algorithm>

class Paddle : public ISceneComponent, public ISceneRender, public ISceneUpdate {
public:
    enum class Direction : int { Left = -1, None = 0, Right = 1 };

    Paddle()
            : bounds(DEFAULT_BOUNDS),
              direction(Direction::None),
              speed(DEFAULT_SPEED) {}

    void onAwake() override {
        graphics = blackboard->getComponent<Graphics>();
    }

    void onUpdate() override {
        bounds.x += static_cast<float>(direction) * speed;

        // clamping to screen borders
        const float maxX = WORLD_WIDTH - bounds.w;
        bounds.x = std::clamp(bounds.x, 0.0f, maxX);
    }

    void onRender() override {
        graphics->drawRectangle(bounds.x, bounds.y, bounds.w, bounds.h,
                                255, 0, 1, 1);
    }

    void onDestroy() override {

    }

    void move(Direction dir) {
        direction = dir;
    }

private:
    static constexpr float WORLD_WIDTH = 100.0f;
    static constexpr float DEFAULT_SPEED = 0.75f;
    inline static const Rect DEFAULT_BOUNDS{40, 1, 20, 5};

    Rect bounds;
    Direction direction;
    float speed;
    std::shared_ptr<Graphics> graphics;
};
