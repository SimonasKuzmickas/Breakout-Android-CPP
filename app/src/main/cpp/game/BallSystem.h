#pragma once

#include <algorithm>
#include "helpers/Math.h"
#include "Paddle.h"
#include "LevelManager.h"
#include <algorithm>

struct Ball {
    Rect bounds;
    Vector2 velocity;

    Ball(float x, float y, float radius, const Vector2& v)
            : bounds{x, y, radius, radius}, velocity(v) {}
};

class BallSystem : public ISceneComponent, public ISceneUpdate {
public:
    enum class BallsType {
        Normal,
        Strong,
        Fire
    };

    Event<> onHitWall;

    void onAwake() override {
        paddle = getComponent<Paddle>();
        levelManager = getComponent<LevelManager>();

        globalSpeedMultiplier = 1;
        ballsType = BallsType::Normal;

        createBall(1000, 30, 30, Vector2(7.0f, 7.0f));
    }

    void onDestroy() override {
        balls.clear();
    }

    void onUpdate() override {
        if (!levelManager)
            return;

        globalSpeedMultiplier += 0.0005f;

        auto& bricks = levelManager->getBricks();
        auto levelBounds = levelManager->getLevelBounds();

        // TODO: Implement Strong Ball!
        for (auto& ball : balls) {
                ball.bounds.x += ball.velocity.x * globalSpeedMultiplier;
                for (auto& brick : levelManager->getBricks()) {
                    if (ball.bounds.overlaps(brick.getBounds())) {
                        ball.bounds.x -= ball.velocity.x;
                        ball.velocity.x = -ball.velocity.x;

                        levelManager->removeBrick((brick));
                        break;
                    }
                }

                ball.bounds.y += ball.velocity.y * globalSpeedMultiplier;
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
                removeBall(ball);
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

    void createBall(float x, float y, float s, Vector2 v) {
        balls.emplace_back(x, y, s, v);
    }

    void removeBall(const Ball& ballRef) {
        auto target = std::find_if(balls.begin(), balls.end(),
           [&](const Ball& b) {
               return &b == &ballRef;
           });

        if (target != balls.end()) {
            balls.erase(target);
        }
    }

    void setBallType(BallsType type)
    {
        ballsType = type;
    }

    BallsType getBallsType() const {
        return ballsType;
    }

    const std::vector<Ball>& getBalls() const {
        return balls;
    }

    void increaseGlobalSpeed(float multiplier)
    {
        globalSpeedMultiplier *= multiplier;
    }

private:
    std::vector<Ball> balls;
    std::shared_ptr<Paddle> paddle;
    std::shared_ptr<LevelManager> levelManager;

    float globalSpeedMultiplier;
    BallsType ballsType;
};
