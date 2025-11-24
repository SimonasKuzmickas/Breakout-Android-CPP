#pragma once
#include "Math.h"
#include <algorithm>

struct Ball {
    Vector2 position;
    Vector2 velocity;
    float radius;

    const float WORLD_WIDTH  = 100.0f;
    const float WORLD_HEIGHT = 100.0f;

    Ball(float x, float y, float r, Vector2 v)
            : position{x, y}, velocity{v}, radius{r} {}

    void update() {
        position += velocity;

        if (position.x - radius < 0.0f) {
            position.x = radius;
            velocity.x = -velocity.x;
        } else if (position.x + radius > WORLD_WIDTH) {
            position.x = WORLD_WIDTH - radius;
            velocity.x = -velocity.x;
        }

// Bounce Y
        if (position.y - radius < 0.0f) {
            position.y = radius;
            velocity.y = -velocity.y;
        } else if (position.y + radius > WORLD_HEIGHT) {
            position.y = WORLD_HEIGHT - radius;
            velocity.y = -velocity.y;
        }
    }

    void render(Graphics& g) {
        //g.drawCircle(position.x, position.y, radius, 255, 255, 255, 1);
    }
};

class BallSystem : public ISceneComponent, public ISceneRender, public ISceneUpdate {
public:

    void createBall(float x, float y, float r, Vector2 v) {
        balls.emplace_back(x, y, r, v);
    }

    void onAwake() override {
        graphics = blackboard->getComponent<Graphics>();

        createBall(50, 50, 1, Vector2(0.5f, 0.3f));
    }

    void onUpdate() override {
        for (auto& ball : balls) {
            ball.update();
        }
    }

    void onRender() override {
        for (auto& ball : balls) {
            ball.render(*graphics);
            graphics->drawRectangle(ball.position.x, ball.position.y, ball.radius, ball.radius,
                                    255, 0, 1, 1);
        }
    }

    void onDestroy() override {
        balls.clear();
    }

private:
    std::vector<Ball> balls;
    std::shared_ptr<Graphics> graphics;

};
