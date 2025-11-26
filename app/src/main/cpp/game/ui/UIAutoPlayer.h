#pragma once

#include <GLES2/gl2.h>
#include "../scene/ISceneComponent.h"

namespace Breakout {

class UIAutoPlayer : public ISceneComponent {
public:

    void onAwake() override {
        paddle = getComponent<Paddle>();
        ballSystem = getComponent<BallSystem>();
    }

    void onDestroy() override {}

    void onUpdate() override {
        auto closestPositionOpt = getClosestBall(*ballSystem, *paddle,
                                                 true); // try downward first

        if (!closestPositionOpt.has_value()) {
            closestPositionOpt = getClosestBall(*ballSystem, *paddle,
                                                false); // fallback to upward
        }

        if (!closestPositionOpt.has_value()) {
            paddle->move(Paddle::Direction::None);
            return;
        }

        Vector2 closestPosition = closestPositionOpt.value();
        float paddleCenterX = paddle->getBounds().center().x;
        float tolerance = 130.0f;

        if (closestPosition.x > paddleCenterX + tolerance) {
            paddle->move(Paddle::Direction::Right);
        } else if (closestPosition.x < paddleCenterX - tolerance) {
            paddle->move(Paddle::Direction::Left);
        } else {
            paddle->move(Paddle::Direction::None);
        }
    }

private:
    std::shared_ptr<Paddle> paddle;
    std::shared_ptr<BallSystem> ballSystem;

    std::optional<Vector2>
    getClosestBall(BallSystem &ballSystem, Paddle &paddle, bool downward) {
        std::optional<Vector2> closestBall;
        float closestDist = std::numeric_limits<float>::max();

        for (const Ball &ball: ballSystem.getBalls()) {
            // Filter by direction
            if (downward && ball.velocity.y < 0.0f) continue; // skip upward
            if (!downward && ball.velocity.y > 0.0f) continue; // skip downward

            float dx = ball.bounds.center().x - paddle.getBounds().center().x;
            float dy = ball.bounds.center().y - paddle.getBounds().center().y;
            float dist = std::sqrt(dx * dx + dy * dy);

            if (dist < closestDist) {
                closestDist = dist;
                closestBall = ball.bounds.center();
            }
        }

        return closestBall;
    }
};

}