#pragma once

namespace Breakout {

struct PiercingBall : Ball {

public:
    PiercingBall(float x,
                 float y,
                 const Vector2& v,
                 BallBlackboard* blackboard)
            : Ball(x, y, v, blackboard) {
        ballType = BallType::Piercing;
    }

    void updateBallMovement() override {
        speedMultiplier += SPEED_GROWTH * GameTime::deltaTime();

        moveBothAxes([this](float& pos, float& vel, bool isXAxis) {
            handlePiercingCollision(blackboard->levelSystem->checkBrickCollision(bounds),
                                    pos, vel, isXAxis);
        });
    }

private:
    void handlePiercingCollision(const std::shared_ptr<Brick>& brick, float& axisPos, float& axisVel, bool isXAxis) {
        if (!brick) return;

        brick->hit();

        if (!brick->getIsDestructible()) {
            Rect brickBounds = brick->getBounds();
            resolveAxisCollision(axisPos, axisVel, brickBounds, isXAxis);
        }
    }
};

}
