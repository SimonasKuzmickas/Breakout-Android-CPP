#pragma once

#include "helpers/Math.h"
#include "Paddle.h"
#include "LevelSystem.h"

struct Ball {
    Rect bounds;
    Vector2 velocity;

    Ball(float x, float y, float radius, const Vector2& v)
            : bounds{x, y, radius, radius}, velocity(v) {}
};

class BallSystem : public ISceneComponent, public ISceneComponentUpdate {
public:
    enum class BallsType {
        Normal,
        Strong,
        Fire
    };

    Event<> onHitWall;
    Event<> onExplosion;

    void onAwake() override {
        paddle = getComponent<Paddle>();
        levelSystem = getComponent<LevelSystem>();

        globalSpeedMultiplier = 1;
        ballsType = BallsType::Normal;

        createBall(1000, 30, 30, Vector2(7.0f, 7.0f));
    }

    void onDestroy() override {
        balls.clear();
    }

    void onUpdate() override {
        if (!levelSystem)
            return;

        globalSpeedMultiplier += SPEED_GROWTH;

        auto& bricks = levelSystem->getBricks();
        auto levelBounds = levelSystem->getLevelBounds();

        for (auto& ball : balls) {

            switch (ballsType) {
                case BallsType::Normal:
                    // --- MOVE X ---
                    ball.bounds.x += ball.velocity.x * globalSpeedMultiplier;
                    if (handleAxisNormalCollision(ball, ball.bounds.x, ball.velocity.x))
                        continue;  // brick hit, done with this ball

                    // --- MOVE Y ---
                    ball.bounds.y += ball.velocity.y * globalSpeedMultiplier;
                    if (handleAxisNormalCollision(ball, ball.bounds.y, ball.velocity.y))
                        continue;

                    break;

                case BallsType::Strong:
                    // --- MOVE XY ---
                    ball.bounds.x += ball.velocity.x * globalSpeedMultiplier;
                    ball.bounds.y += ball.velocity.y * globalSpeedMultiplier;

                    if (Brick* hit = levelSystem->checkBrickCollision(ball.bounds))
                    {
                        levelSystem->removeBrick(*hit);
                    }
                    break;

                case BallsType::Fire:
                    // --- MOVE X ---
                    ball.bounds.x += ball.velocity.x * globalSpeedMultiplier;
                    if (handleAxisFireCollision(ball, ball.bounds.x, ball.velocity.x))
                        continue;  // brick hit, done with this ball

                    // --- MOVE Y ---
                    ball.bounds.y += ball.velocity.y * globalSpeedMultiplier;
                    if (handleAxisFireCollision(ball, ball.bounds.y, ball.velocity.y))
                        continue;

                    break;
            }

            // ---- OUT OF BOUNDS
            if (!ball.bounds.overlaps(levelSystem->getLevelBounds())) {
                // TODO: Game Over
                removeBall(ball);
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
            if(ball.velocity.y < 0)
            {
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
    std::shared_ptr<LevelSystem> levelSystem;

    float globalSpeedMultiplier;
    BallsType ballsType;

    static constexpr float SPEED_GROWTH = 0.0005f;

    bool handleAxisNormalCollision(Ball& ball, float& axisPos, float& axisVel)
    {
        if (Brick* hit = levelSystem->checkBrickCollision(ball.bounds))
        {
            axisPos -= axisVel;
            axisVel = -axisVel;
            levelSystem->removeBrick(*hit);
            return true;
        }
        return false;
    }

    bool handleAxisFireCollision(Ball& ball, float& axisPos, float& axisVel)
    {
        if (Brick* hit = levelSystem->checkBrickCollision(ball.bounds))
        {
            onExplosion.invoke();

            auto brickBounds = hit->getBounds();

            axisPos -= axisVel;
            axisVel = -axisVel;
            levelSystem->removeBrick(*hit);

            auto explosionBounds = Rect(brickBounds.x - 50, brickBounds.y - 50,
                                        brickBounds.w + 100, brickBounds.h + 100);

            auto& bricks = levelSystem->getBricks();
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
