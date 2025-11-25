#pragma once

#include <algorithm>
#include "helpers/Math.h"
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include "LevelManager.h"
#include <algorithm>

class BallSystem : public ISceneComponent, public ISceneRender, public ISceneUpdate {
public:
    Event<> onHitWall;

    void onAwake() override {
        graphics = getComponent<Graphics>();
        paddle = getComponent<Paddle>();
        levelManager = getComponent<LevelManager>();

        createBall(1000, 30, 30, Vector2(7.0f, 7.0f));
    }

    void onUpdate() override {
        if (!levelManager)
            return;

        auto& bricks = levelManager->getBricks();
        auto levelBounds = levelManager->getLevelBounds();

        for (auto& ball : balls) {
                ball.bounds.x += ball.velocity.x;
                for (auto& brick : levelManager->getBricks()) {
                    if (ball.bounds.overlaps(brick.getBounds())) {
                        ball.bounds.x -= ball.velocity.x;
                        ball.velocity.x = -ball.velocity.x;
                        levelManager->removeBrick((brick));
                        break;
                    }
                }

                ball.bounds.y += ball.velocity.y;
                for (auto& brick : levelManager->getBricks()) {
                    if (ball.bounds.overlaps(brick.getBounds())) {
                        ball.bounds.y -= ball.velocity.y;
                        ball.velocity.y = -ball.velocity.y;
                        levelManager->removeBrick((brick));
                        break;
                    }
                }

            if (ball.bounds.y < 0.0f) {
                // TODO: Game Over
                return;
            }

            if (ball.bounds.x < levelBounds.left()) {
                ball.bounds.x = levelBounds.left();
                ball.velocity.x = -ball.velocity.x;

                onHitWall.invoke();
            } else if (ball.bounds.x + ball.bounds.w > levelBounds.right()) {
                ball.bounds.x = levelBounds.right() - ball.bounds.w;
                ball.velocity.x = -ball.velocity.x;

                onHitWall.invoke();
            }

            if (ball.bounds.y + ball.bounds.h > levelBounds.top()) {
                ball.bounds.y = levelBounds.top() - ball.bounds.h;
                ball.velocity.y = -ball.velocity.y;

                onHitWall.invoke();
            }

            Rect paddleBounds = paddle->getBounds();
            if (paddle && ball.bounds.overlaps(paddleBounds)) {
                float normalized = ((ball.bounds.x + ball.bounds.w * 0.5f) - (paddleBounds.x + paddleBounds.w * 0.5f))
                                   / (paddleBounds.w * 0.5f);

                float speed = std::sqrt(ball.velocity.x * ball.velocity.x + ball.velocity.y * ball.velocity.y);

                float len = std::sqrt(normalized * normalized + 1.0f);
                float dirX = normalized / len;
                float dirY = 1.0f / len;

                ball.velocity.x = dirX * speed;
                ball.velocity.y = dirY * speed;

                paddle->onHit.invoke();
            }
        }
    }

    void onRender() override {
        for (auto& ball : balls) {
            graphics->drawImage(graphics->resourceBall, ball.bounds.x, ball.bounds.y, ball.bounds.w, ball.bounds.h,1, 1, 1, 1);
        }
    }

    void onDestroy() override {
        balls.clear();
    }

    void createBall(float x, float y, float s, Vector2 v) {
        balls.emplace_back(x, y, s, v);
    }

private:
    std::vector<Ball> balls;
    std::shared_ptr<Graphics> graphics;
    std::shared_ptr<Paddle> paddle;
    std::shared_ptr<LevelManager> levelManager;
};
