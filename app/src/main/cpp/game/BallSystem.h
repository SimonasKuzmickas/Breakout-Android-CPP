#pragma once
#include "Math.h"
#include "Ball.h"
#include <algorithm>

class BallSystem : public ISceneComponent, public ISceneRender, public ISceneUpdate {
public:

    void createBall(float x, float y, float s, Vector2 v) {
        balls.emplace_back(x, y, s, v, paddle);
    }

    void onAwake() override {
        graphics = blackboard->getComponent<Graphics>();
        paddle = blackboard->getComponent<Paddle>();

        createBall(1000, 50, 30, Vector2(10.0f, 10.0f));
    }

    void onUpdate() override {
        for (auto& ball : balls) {
            ball.update();
        }
    }

    void onRender() override {
        for (auto& ball : balls) {
            graphics->drawRectangle(ball.getBounds().x, ball.getBounds().y, ball.getBounds().w, ball.getBounds().h,255, 0, 1, 1);
        }
    }

    void onDestroy() override {
        balls.clear();
    }

private:
    std::vector<Ball> balls;
    std::shared_ptr<Graphics> graphics;
    std::shared_ptr<Paddle> paddle;
};
