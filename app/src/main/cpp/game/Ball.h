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

        Rect paddleBounds = paddle->getBounds();
        if (paddle && bounds.overlaps(paddleBounds)) {
            float ballCenterX   = bounds.x + bounds.w * 0.5f;
            float paddleCenterX = paddleBounds.x + paddleBounds.w * 0.5f;
            float offset        = ballCenterX - paddleCenterX;

            float normalized = offset / (paddleBounds.w * 0.5f);
            float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

            Vector2 newDir(normalized, 1.0f);

            float len = std::sqrt(newDir.x * newDir.x + newDir.y * newDir.y);
            newDir.x /= len;
            newDir.y /= len;

            velocity.x = newDir.x * speed;
            velocity.y = newDir.y * speed;
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
