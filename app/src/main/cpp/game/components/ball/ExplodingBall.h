#pragma once

namespace Breakout {

struct ExplodingBall : Ball {
    ExplodingBall(float x,
                  float y,
                  const Vector2& v,
                  BallBlackboard* blackboard)
            : Ball(x, y, v, blackboard) {
        ballType = BallType::Explode;
    }

    void updateBallMovement() override {
        speedMultiplier += SPEED_GROWTH * GameTime::deltaTime();

        moveBothAxes([this](float& pos, float& vel, bool) {
            if (auto brick = handleCollision(pos, vel)) {
                if (brick->getIsDestructible()) {
                    blackboard->levelSystem->explode(brick->getGridX(), brick->getGridY());
                }
            }
        });
    }
};

}
