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
        if (levelManager) {
            auto& bricks = levelManager->getBricks();

            bounds.x += velocity.x;
            for (auto& brick : levelManager->getBricks()) {
                if (bounds.overlaps(brick.getBounds())) {
                    bounds.x -= velocity.x;
                    velocity.x = -velocity.x;
                    levelManager->removeBrick((brick));
                    break;
                }
            }

            bounds.y += velocity.y;
            for (auto& brick : levelManager->getBricks()) {
                if (bounds.overlaps(brick.getBounds())) {
                    bounds.y -= velocity.y;
                    velocity.y = -velocity.y;
                    levelManager->removeBrick((brick));
                    break;
                }
            }
        }

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

            paddle->hitBall();
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
