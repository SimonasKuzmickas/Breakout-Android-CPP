#pragma once

#include "helpers/Math.h"
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

        auto levelBounds = levelManager->getLevelBounds();
        if (bounds.x < levelBounds.left()) {
            bounds.x = levelBounds.left();
            velocity.x = -velocity.x;

            levelManager->onHitEdge.invoke();
        } else if (bounds.x + bounds.w > levelBounds.right()) {
            bounds.x = levelBounds.right() - bounds.w;
            velocity.x = -velocity.x;

            levelManager->onHitEdge.invoke();
        }

        if (bounds.y + bounds.h > levelBounds.top()) {
            bounds.y = levelBounds.top() - bounds.h;
            velocity.y = -velocity.y;

            levelManager->onHitEdge.invoke();
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

            paddle->onHit.invoke();
        }
    }

    Rect getBounds() { return bounds; }

private:

    Rect bounds;
    Vector2 velocity;
    std::shared_ptr<Paddle> paddle;
    std::shared_ptr<LevelManager> levelManager;
};
