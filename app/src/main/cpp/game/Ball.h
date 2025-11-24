#pragma once
#include "Math.h"
#include "Paddle.h"
#include <algorithm>

struct Ball {
    Ball(float x, float y, float r, Vector2 v, const std::shared_ptr<Paddle>& pad)
            : bounds{x, y, r, r}, velocity{v}, paddle{pad} {}

public:

    void update() {
        bounds.x += velocity.x;
        bounds.y += velocity.y;

        if (bounds.y < 0.0f) {
            // TODO: Game Over
            return;
        }

        if (bounds.x < 0.0f) {
            bounds.x = 0;
            velocity.x = -velocity.x;
        } else if (bounds.x + bounds.w > WORLD_WIDTH) {
            bounds.x = WORLD_WIDTH - bounds.w;
            velocity.x = -velocity.x;
        }

        if (bounds.y + bounds.h > WORLD_HEIGHT) {
            bounds.y = WORLD_HEIGHT - bounds.h;
            velocity.y = -velocity.y;
        }

        if (paddle && bounds.overlaps(paddle->getBounds())) {
            velocity.y = -velocity.y;
        }
    }

    Rect getBounds() { return bounds; }

private:

    Rect bounds;
    Vector2 velocity;
    std::shared_ptr<Paddle> paddle;

    const float WORLD_WIDTH  = 1920.0f;
    const float WORLD_HEIGHT = 1080.0f;
};
