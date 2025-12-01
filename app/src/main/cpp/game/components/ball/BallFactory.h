#pragma once

#include "BallBlackboard.h"
#include "BallType.h"
#include "Ball.h"
#include "PiercingBall.h"
#include "ExplodingBall.h"

namespace Breakout {

struct BallFactory {
    static std::unique_ptr<Ball> createBall(
            float x,
            float y,
            const Vector2& velocity,
            BallType type,
            BallBlackboard* blackboard) {
        switch (type) {
            case BallType::Normal:
                return std::make_unique<Ball>(x, y, velocity, blackboard);

            case BallType::Piercing:
                return std::make_unique<PiercingBall>(x, y, velocity, blackboard);

            case BallType::Explode:
                return std::make_unique<ExplodingBall>(x, y, velocity, blackboard);

            default:
                return std::make_unique<Ball>(x, y, velocity, blackboard);
        }
    }
};


}
