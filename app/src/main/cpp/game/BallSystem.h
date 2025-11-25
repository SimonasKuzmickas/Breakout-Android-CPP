#pragma once

#include <algorithm>
#include "helpers/Math.h"
#include "Ball.h"

class BallSystem : public ISceneComponent, public ISceneRender, public ISceneUpdate {
public:

    void onAwake() override {
        graphics = getComponent<Graphics>();
        paddle = getComponent<Paddle>();
        levelManager = getComponent<LevelManager>();

        createBall(1000, 30, 30, Vector2(7.0f, 7.0f));
    }

    void onUpdate() override {
        for (auto& ball : balls) {
            ball.update();
        }
    }

    void onRender() override {
        for (auto& ball : balls) {
            graphics->drawRectangle(ball.getBounds().x, ball.getBounds().y, ball.getBounds().w, ball.getBounds().h,1, 1, 1, 1);
        }
    }

    void onDestroy() override {
        balls.clear();
    }

    void createBall(float x, float y, float s, Vector2 v) {
        balls.emplace_back(x, y, s, v, paddle, levelManager);
    }

private:
    std::vector<Ball> balls;
    std::shared_ptr<Graphics> graphics;
    std::shared_ptr<Paddle> paddle;
    std::shared_ptr<LevelManager> levelManager;
};
