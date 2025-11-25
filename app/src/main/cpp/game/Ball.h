#pragma once
#include "Math.h"
#include "Paddle.h"
#include "Brick.h"
#include "LevelManager.h"
#include <algorithm>

struct Ball {
    Ball(float x, float y, float r, Vector2 v,
         std::shared_ptr<Paddle>& pad,
         std::shared_ptr<LevelManager>& level)
            : bounds{x, y, r, r}, velocity{v},
            paddle{pad},
            levelManager{level} {}

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
            float normalized = ((bounds.x + bounds.w * 0.5f) - (paddleBounds.x + paddleBounds.w * 0.5f))
                               / (paddleBounds.w * 0.5f);

            float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

            float len = std::sqrt(normalized * normalized + 1.0f);
            float dirX = normalized / len;
            float dirY = 1.0f / len;

            velocity.x = dirX * speed;
            velocity.y = dirY * speed;
        }

        if (levelManager) {
            auto& bricks = levelManager->getBricks();

            for (auto& brick : bricks) {
                Rect brickBounds = brick.getBounds();
                if (bounds.overlaps(brickBounds)) {

                    float overlapLeft   = (bounds.x + bounds.w) - brickBounds.x;
                    float overlapRight  = (brickBounds.x + brickBounds.w) - bounds.x;
                    float overlapTop    = (bounds.y + bounds.h) - brickBounds.y;
                    float overlapBottom = (brickBounds.y + brickBounds.h) - bounds.y;
                    float minOverlapX = std::min(overlapLeft, overlapRight);
                    float minOverlapY = std::min(overlapTop, overlapBottom);
                    if (minOverlapX < minOverlapY) {
                        // Resolve X
                        if (overlapLeft < overlapRight)
                            bounds.x -= minOverlapX;
                        else
                            bounds.x += minOverlapX;

                        velocity.x = -velocity.x;
                    } else {
                        // Resolve Y
                        if (overlapTop < overlapBottom)
                            bounds.y -= minOverlapY;
                        else
                            bounds.y += minOverlapY;

                        velocity.y = -velocity.y;
                    }
                    break; // only handle one brick per frame
                }
            }

        }
    }

    Rect getBounds() { return bounds; }

private:

    Rect bounds;
    Vector2 velocity;
    std::shared_ptr<Paddle> paddle;
    std::shared_ptr<LevelManager> levelManager;

    const float WORLD_WIDTH  = 1920.0f;
    const float WORLD_HEIGHT = 1080.0f;
};
