#pragma once

#include <algorithm>
#include "../helpers/Rect.h"
#include "../helpers/Event.h"
#include "../scene/ISceneComponent.h"

namespace Breakout {

class Paddle : public ISceneComponent {
public:
    enum class Direction : int {
        Left = -1, None = 0, Right = 1
    };

    Event<> onHit;

    Paddle()
            : bounds(DEFAULT_BOUNDS),
              direction(Direction::None),
              speed(DEFAULT_SPEED),
              transitionWidth(DEFAULT_BOUNDS.w) {}

    void onUpdate() override {
        bounds.x += static_cast<float>(direction) * speed * GameTime::deltaTime();

        // Clamp to screen bounds
        const float maxX = WORLD_WIDTH - bounds.w;
        bounds.x = std::clamp(bounds.x, 0.0f, maxX);

        // Smooth resize animation
        float difference = transitionWidth - bounds.w;
        difference *= RESIZE_SPEED * GameTime::deltaTime();
        bounds.x -= difference * 0.5f;
        bounds.w += difference;

        // Minimum width
        if (bounds.w < MIN_SIZE) {
            bounds.w = MIN_SIZE;
        }
    }

    // Set movement direction
    void move(Direction dir) {
        direction = dir;
    }

    // Reset paddle to default
    void start() {
        bounds = DEFAULT_BOUNDS;
        transitionWidth = bounds.w;
    }

    // Request a width increase for animation
    void expand(float increase) {
        transitionWidth += increase;
    }

    Rect getBounds() { return bounds; }

private:
    static constexpr float WORLD_WIDTH = 1920.0f;
    static constexpr float MIN_SIZE = 120.0f;
    static constexpr float RESIZE_SPEED = 18.0f;
    static constexpr float DEFAULT_SPEED = 1200.0f;
    inline static const Rect DEFAULT_BOUNDS{1920.0f / 2 - 250, 20, 300, 60};

    Rect bounds;
    Direction direction;
    float speed;
    float transitionWidth = 0;
};

}