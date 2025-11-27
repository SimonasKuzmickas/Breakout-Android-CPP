#pragma once

#include "Paddle.h"
#include "LevelSystem.h"

namespace Breakout {

struct Ball {
    static constexpr float BALL_SIZE = 30.0f;

    Rect bounds;
    Vector2 velocity;

    Ball(float x, float y, const Vector2 &v)
            : bounds{x, y, BALL_SIZE, BALL_SIZE}, velocity(v) {}

    void applySpeedMultiplier(float multiplier) {
        velocity.x *= multiplier;
        velocity.y *= multiplier;
    }
};

class BallSystem : public ISceneComponent {
public:
    enum class BallsType { Normal, Strong, Explode };

    Event<> onHitWall;
    Event<> onExplosion;
    Event<> onLost;

    void onAwake() override {
        paddle = getComponent<Paddle>();
        levelSystem = getComponent<LevelSystem>();
        levelSystem->onLevelStart.addListener([this]() {
            start();
        });

        start();
    }

    void start() {
        balls.clear();
        globalSpeedMultiplier = 1.0f;
        ballsType = BallsType::Normal;

        createBall(WORLD_WIDTH * 0.5f, START_Y , randomBallStartingVelocity(SPEED_START));

        paddle->start();
    }

    void onUpdate() override {
        if (!levelSystem) return;

        globalSpeedMultiplier += SPEED_GROWTH * GameTime::deltaTime();
        auto levelBounds = levelSystem->getLevelBounds();

        for (auto &ball: balls) {

            switch (ballsType) {
                case BallsType::Normal:
                    // --- MOVE X ---
                    ball.bounds.x += ball.velocity.x * globalSpeedMultiplier * GameTime::deltaTime();
                    if (handleAxisNormalCollision(ball, ball.bounds.x, ball.velocity.x))
                        continue;  // brick hit, done with this ball

                    // --- MOVE Y ---
                    ball.bounds.y += ball.velocity.y * globalSpeedMultiplier * GameTime::deltaTime();
                    if (handleAxisNormalCollision(ball, ball.bounds.y, ball.velocity.y))
                        continue;

                    break;

                case BallsType::Strong:
                    // --- MOVE XY ---
                    ball.bounds.x += ball.velocity.x * globalSpeedMultiplier * GameTime::deltaTime();
                    ball.bounds.y += ball.velocity.y * globalSpeedMultiplier * GameTime::deltaTime();

                    if (Brick *hit = levelSystem->checkBrickCollision(ball.bounds)) {
                        levelSystem->removeBrick(*hit);
                    }
                    break;

                case BallsType::Explode:
                    // --- MOVE X ---
                    ball.bounds.x += ball.velocity.x * globalSpeedMultiplier * GameTime::deltaTime();
                    if (handleAxisFireCollision(ball, ball.bounds.x, ball.velocity.x))
                        continue;

                    // --- MOVE Y ---
                    ball.bounds.y += ball.velocity.y * globalSpeedMultiplier * GameTime::deltaTime();
                    if (handleAxisFireCollision(ball, ball.bounds.y, ball.velocity.y))
                        continue;

                    break;
            }

            // ---- OUT OF BOUNDS
            if (ball.bounds.y < 0) {
                removeBall(ball);
                if (balls.empty()) {
                    onLost.invoke();
                    start();
                }
            }

            // ---- WALL COLLISIONS ----
            // LEFT
            if (ball.bounds.x < levelBounds.left()) {
                ball.bounds.x = levelBounds.left();
                ball.velocity.x = -ball.velocity.x;

                onHitWall.invoke();
            // RIGHT
            } else if (ball.bounds.x + ball.bounds.w > levelBounds.right()) {
                ball.bounds.x = levelBounds.right() - ball.bounds.w;
                ball.velocity.x = -ball.velocity.x;

                onHitWall.invoke();
            }

            // TOP
            if (ball.bounds.y + ball.bounds.h > levelBounds.top()) {
                ball.bounds.y = levelBounds.top() - ball.bounds.h;
                ball.velocity.y = -ball.velocity.y;

                onHitWall.invoke();
            }

            // ---- PADDLE COLLISION ----
            if (ball.velocity.y < 0) {
                Rect paddleBounds = paddle->getBounds();
                if (paddle && ball.bounds.overlaps(paddleBounds)) {
                    float normalized = ((ball.bounds.x + ball.bounds.w * 0.5f) -
                                        (paddleBounds.x + paddleBounds.w * 0.5f))
                                       / (paddleBounds.w * 0.5f);

                    float speed = std::sqrt(ball.velocity.x * ball.velocity.x +
                                            ball.velocity.y * ball.velocity.y);

                    float len = std::sqrt(normalized * normalized + 1.0f);
                    float dirX = normalized / len;
                    float dirY = 1.0f / len;

                    ball.velocity.x = dirX * speed;
                    ball.velocity.y = dirY * speed;
                    ball.applySpeedMultiplier(SPEED_HITGROWTH);

                    paddle->onHit.invoke();
                }
            }
        }
    }

    void createBall(float x, float y, Vector2 v) {
        balls.emplace_back(x, y, v);
    }

    void removeBall(const Ball &ballRef) {
        auto target = std::find_if(balls.begin(), balls.end(),
               [&](const Ball &b) {
                   return &b == &ballRef;
               });

        if (target != balls.end()) {
            balls.erase(target);
        }
    }

    void setBallType(BallsType type) {
        ballsType = type;
    }

    BallsType getBallsType() const {
        return ballsType;
    }

    const std::vector<Ball> &getBalls() const {
        return balls;
    }

    void increaseGlobalSpeed(float multiplier) {
        globalSpeedMultiplier *= multiplier;
    }

private:
    static constexpr float SPEED_START = 600.0f;
    static constexpr float SPEED_GROWTH = 0.03f;
    static constexpr float SPEED_HITGROWTH = 1.01f;
    static constexpr float WORLD_WIDTH = 1920.0f;
    static constexpr float START_Y = 200.0f;

    std::vector<Ball> balls;
    std::shared_ptr<Paddle> paddle;
    std::shared_ptr<LevelSystem> levelSystem;

    float globalSpeedMultiplier;
    BallsType ballsType;

    Vector2 randomBallStartingVelocity(float speed) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));

        float minAngle = M_PI / 4.0f;       // 45°
        float maxAngle = 3.0f * M_PI / 4.0f; // 135°
        float angle = minAngle + (static_cast<float>(std::rand()) / RAND_MAX) * (maxAngle - minAngle);

        // Build vector directly from angle
        float vx = speed * std::cos(angle);
        float vy = speed * std::sin(angle);

        return {vx, vy};
    }

    bool handleAxisNormalCollision(Ball &ball, float &axisPos, float &axisVel) {
        if (Brick *hit = levelSystem->checkBrickCollision(ball.bounds)) {
            axisPos -= axisVel * GameTime::deltaTime();
            axisVel = -axisVel;
            ball.applySpeedMultiplier(SPEED_HITGROWTH);
            levelSystem->removeBrick(*hit);
            return true;
        }
        return false;
    }

    bool handleAxisFireCollision(Ball &ball, float &axisPos, float &axisVel) {
        if (Brick *hit = levelSystem->checkBrickCollision(ball.bounds)) {
            onExplosion.invoke();

            auto brickBounds = hit->getBounds();

            axisPos -= axisVel * GameTime::deltaTime();
            axisVel = -axisVel;
            ball.applySpeedMultiplier(SPEED_HITGROWTH);
            levelSystem->removeBrick(*hit);

            auto explosionBounds = Rect(brickBounds.x - 50, brickBounds.y - 50,
                                        brickBounds.w + 100, brickBounds.h + 100);

            auto &bricks = levelSystem->getBricks();
            std::vector<int> toRemove;

            for (int i = 0; i < static_cast<int>(bricks.size()); ++i) {
                if (explosionBounds.overlaps(bricks[i].getBounds())) {
                    toRemove.push_back(i);
                }
            }

            for (int i = static_cast<int>(toRemove.size()) - 1; i >= 0; --i) {
                int idx = toRemove[i];
                levelSystem->removeBrick(bricks[idx]); // or removeBrickByIndex(idx)
            }

            return true;
        }

        return false;
    }
};

}