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
    enum class BallsType { Normal, Piercing, Explode };

    Event<> onHitWall;
    Event<> onLost;

    BallSystem(std::shared_ptr<Paddle>  paddle, std::shared_ptr<LevelSystem>  levelSystem)
            : paddle(paddle), levelSystem(levelSystem)
    {}

    void onAwake() override {
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
                    if (handleCollision(ball, ball.bounds.x, ball.velocity.x))
                        continue;

                    // --- MOVE Y ---
                    ball.bounds.y += ball.velocity.y * globalSpeedMultiplier * GameTime::deltaTime();
                    if (handleCollision(ball, ball.bounds.y, ball.velocity.y))
                        continue;

                    break;

                case BallsType::Explode:
                    // --- MOVE X ---
                    ball.bounds.x += ball.velocity.x * globalSpeedMultiplier * GameTime::deltaTime();
                    if (Brick* brick = handleCollision(ball, ball.bounds.x, ball.velocity.x)) {
                        if(brick->getIsDestructible()) {
                            levelSystem->explode(brick->getGridX(), brick->getGridY());
                        }
                        continue;
                    }

                    // --- MOVE Y ---
                    ball.bounds.y += ball.velocity.y * globalSpeedMultiplier * GameTime::deltaTime();
                    if (Brick* brick = handleCollision(ball, ball.bounds.y, ball.velocity.y)) {
                        if(brick->getIsDestructible()) {
                            levelSystem->explode(brick->getGridX(), brick->getGridY());
                        }
                        continue;
                    }
                    break;

                case BallsType::Piercing:
                    // --- MOVE X ---
                    ball.bounds.x += ball.velocity.x * globalSpeedMultiplier * GameTime::deltaTime();
                    handlePiercingCollision(ball,
                                          levelSystem->checkBrickCollision(ball.bounds),
                                          ball.bounds.x, ball.velocity.x,
                                          true);

                    // --- MOVE Y ---
                    ball.bounds.y += ball.velocity.y * globalSpeedMultiplier * GameTime::deltaTime();
                    handlePiercingCollision(ball,
                                          levelSystem->checkBrickCollision(ball.bounds),
                                          ball.bounds.y, ball.velocity.y,
                                          false);

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
                auto paddleBounds = paddle->getBounds();
                if (paddle && ball.bounds.overlaps(paddleBounds)) {
                    float normalized = ((ball.bounds.x + ball.bounds.w * 0.5f) -
                                        (paddleBounds.x + paddleBounds.w * 0.5f))
                                       / (paddleBounds.w * 0.5f);

                    float edgeFactor = 1.5f;
                    normalized *= edgeFactor;

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
    static constexpr float START_Y = 10.0f;

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

    Brick* handleCollision(Ball &ball, float &axisPos, float &axisVel) {
        if (Brick* brick = levelSystem->checkBrickCollision(ball.bounds)) {
            Rect brickBounds = brick->getBounds();

            if (axisVel > 0) {
                if (&axisPos == &ball.bounds.x)
                    axisPos = brickBounds.x - ball.bounds.w;
                else
                    axisPos = brickBounds.y - ball.bounds.h;
            } else {
                if (&axisPos == &ball.bounds.x)
                    axisPos = brickBounds.x + brickBounds.w;
                else
                    axisPos = brickBounds.y + brickBounds.h;
            }

            axisVel = -axisVel;
            ball.applySpeedMultiplier(SPEED_HITGROWTH);
            brick->hit();

            return brick;
        }

        return nullptr;
    }

    void handlePiercingCollision(Ball& ball, Brick* brick, float& axisPos, float& axisVel, bool isXAxis) {
        if (!brick) return;

        if (brick->getIsDestructible()) {
            brick->hit();
        } else {
            auto brickBounds = brick->getBounds();

            if (isXAxis) {
                if (axisVel > 0)
                    axisPos = brickBounds.x - ball.bounds.w;
                else
                    axisPos = brickBounds.x + brickBounds.w;
            } else {
                if (axisVel > 0)
                    axisPos = brickBounds.y - ball.bounds.h;
                else
                    axisPos = brickBounds.y + brickBounds.h;
            }

            axisVel = -axisVel;
        }
    }
};

}