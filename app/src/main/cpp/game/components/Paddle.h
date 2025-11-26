#pragma once

#include <algorithm>
#include "../helpers/Math.h"
#include "../helpers/Event.h"
#include "../scene/ISceneComponent.h"

class Paddle : public ISceneComponent {
public:
    enum class Direction : int { Left = -1, None = 0, Right = 1 };

    Event<> onHit;

    Paddle()
            : bounds(DEFAULT_BOUNDS),
              direction(Direction::None),
              speed(DEFAULT_SPEED),
              transitionWidth(DEFAULT_BOUNDS.w){}

    void onAwake() override {

    }

    void onDestroy() override {

    }

    void onUpdate() override {
        bounds.x += static_cast<float>(direction) * speed;

        // paddle to wall collision
        const float maxX = WORLD_WIDTH - bounds.w;
        bounds.x = std::clamp(bounds.x, 0.0f, maxX);

        // paddle resize animation
        float difference = transitionWidth - bounds.w;
        difference *= 0.3f;

        bounds.x -= difference * 0.5f;
        bounds.w += difference;

        if(bounds.w < 120) {
            bounds.w = 120;
        }
    }

    void move(Direction dir) {
        direction = dir;
    }

    void start()
    {
        bounds = DEFAULT_BOUNDS;
        transitionWidth = bounds.w;
    }

    void expand(float increase)
    {
        transitionWidth += increase;
    }

    Rect getBounds() { return bounds; }

private:
    static constexpr float WORLD_WIDTH = 1920.0f;
    static constexpr float DEFAULT_SPEED = 20.0f;
    inline static const Rect DEFAULT_BOUNDS{1920.0f / 2 - 250, 20, 300, 60};

    Rect bounds;
    Direction direction;
    float speed;
    float transitionWidth = 0;
};
