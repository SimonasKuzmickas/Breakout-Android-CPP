#pragma once

namespace Breakout {



struct Ball {
    static constexpr float SPEED_GROWTH = 0.03f;
    static constexpr float SPEED_HITGROWTH = 1.01f;
    static constexpr float BALL_SIZE = 30.0f;

    Event<> onHitWall;
    Event<> onOutOfScreen;
    Rect bounds;
    Vector2 velocity;
    float speedMultiplier;
    BallType ballType = BallType::Normal;

    Ball(float x, float y, const Vector2 &v, BallBlackboard* ballBlackboard)
            : bounds{x, y, BALL_SIZE, BALL_SIZE}, ballType(BallType::Normal), velocity(v), blackboard(ballBlackboard), speedMultiplier(1.0f) {
    }

    virtual void updateBallMovement() {
        speedMultiplier += SPEED_GROWTH * GameTime::deltaTime();

        moveBothAxes([this](float& pos, float& vel, bool) {
            handleCollision(pos, vel);
        });
    }

    void updateBallCollisions() {
        auto levelBounds = blackboard->levelSystem->getLevelBounds();

        // ---- OUT OF BOUNDS
        if (bounds.y < 0) {
            onOutOfScreen.invoke();
        }

        // ---- WALL COLLISIONS ----
        // LEFT
        if (bounds.x < levelBounds.left()) {
            bounds.x = levelBounds.left();
            velocity.x = - velocity.x;
            onHitWall.invoke();
            // RIGHT
        } else if (bounds.x + bounds.w > levelBounds.right()) {
            bounds.x = levelBounds.right() - bounds.w;
            velocity.x = -velocity.x;
            onHitWall.invoke();
        }

        // TOP
        if (bounds.y + bounds.h > levelBounds.top()) {
            bounds.y = levelBounds.top() - bounds.h;
            velocity.y = -velocity.y;
            onHitWall.invoke();
        }

        // ---- PADDLE COLLISION ----
        if (velocity.y < 0) {
            auto paddleBounds = blackboard->paddle->getBounds();
            if (blackboard->paddle && bounds.overlaps(paddleBounds)) {
                float normalized = ((bounds.x + bounds.w * 0.5f) -
                                    (paddleBounds.x + paddleBounds.w * 0.5f))
                                   / (paddleBounds.w * 0.5f);

                float edgeFactor = 1.5f;
                normalized *= edgeFactor;

                float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

                float len = std::sqrt(normalized * normalized + 1.0f);
                float dirX = normalized / len;
                float dirY = 1.0f / len;

                velocity.x = dirX * speed;
                velocity.y = dirY * speed;
                applySpeedMultiplier(SPEED_HITGROWTH);

                blackboard->paddle->onHit.invoke();
            }
        }
    }

protected:
    BallBlackboard* blackboard;

    void applySpeedMultiplier(float multiplier) {
        velocity.x *= multiplier;
        velocity.y *= multiplier;
    }

    void moveBothAxes(std::function<void(float&, float&, bool)> collisionHandler) {
        // X axis
        moveAxis(true, [&](float& pos, float& vel) {
            collisionHandler(pos, vel, true);
        });

        // Y axis
        moveAxis(false, [&](float& pos, float& vel) {
            collisionHandler(pos, vel, false);
        });
    }

    void moveAxis(bool isXAxis, std::function<void(float&, float&)> collisionHandler) {
        float& axisPos = isXAxis ? bounds.x : bounds.y;
        float& axisVel = isXAxis ? velocity.x : velocity.y;

        axisPos += axisVel * speedMultiplier * GameTime::deltaTime();

        collisionHandler(axisPos, axisVel);
    }

    std::shared_ptr<Brick> handleCollision(float& axisPos, float& axisVel) {
        if (auto brick = blackboard->levelSystem->checkBrickCollision(bounds)) {
            Rect brickBounds = brick->getBounds();

            bool isXAxis = (&axisPos == &bounds.x);
            resolveAxisCollision(axisPos, axisVel, brickBounds, isXAxis);

            applySpeedMultiplier(SPEED_HITGROWTH);
            brick->hit();

            return brick;
        }
        return nullptr;
    }

    void resolveAxisCollision(float& axisPos, float& axisVel, const Rect& brickBounds, bool isXAxis) {
        if (isXAxis) {
            if (axisVel > 0)
                axisPos = brickBounds.x - bounds.w;
            else
                axisPos = brickBounds.x + brickBounds.w;
        } else {
            if (axisVel > 0)
                axisPos = brickBounds.y - bounds.h;
            else
                axisPos = brickBounds.y + brickBounds.h;
        }

        axisVel = -axisVel;
    }
};

}
