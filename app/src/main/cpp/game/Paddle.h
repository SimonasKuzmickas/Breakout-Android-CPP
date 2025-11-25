#pragma once

#include <algorithm>
#include <android/log.h>
#include "helpers/Math.h"
#include "helpers/Event.h"
#include "scene/ISceneComponent.h"
#include "scene/ISceneRender.h"
#include "scene/ISceneUpdate.h"
#include "Graphics.h"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Breakout", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Breakout", __VA_ARGS__)

class Paddle : public ISceneComponent, public ISceneRender, public ISceneUpdate {
public:
    enum class Direction : int { Left = -1, None = 0, Right = 1 };

    Event<> onHit;

    Paddle()
            : bounds(DEFAULT_BOUNDS),
              direction(Direction::None),
              speed(DEFAULT_SPEED) {}

    void onAwake() override {
        graphics = getComponent<Graphics>();
    }

    void onUpdate() override {
        bounds.x += static_cast<float>(direction) * speed;

        // paddle to wall collision
        const float maxX = WORLD_WIDTH - bounds.w;
        bounds.x = std::clamp(bounds.x, 0.0f, maxX);
    }

    void onRender() override {
        paddleAnimation = std::fmod(paddleAnimation + 0.2f, 3.0f);

        float cornerWidth  = 50.0f;
        graphics->drawImage(graphics->resourcePaddles[(int)paddleAnimation], bounds.x + cornerWidth, bounds.y, bounds.w - cornerWidth * 2, bounds.h);
        graphics->drawImage(graphics->resourcePaddleLeft, bounds.x, bounds.y, cornerWidth, bounds.h);
        graphics->drawImage(graphics->resourcePaddleRight, bounds.x + bounds.w - cornerWidth, bounds.y, cornerWidth, bounds.h);
    }

    void onDestroy() override {

    }

    void move(Direction dir) {
        direction = dir;
    }

    void expand(float increase)
    {
        bounds.x -= increase * 0.5f;
        bounds.w += increase;

        if(bounds.w < 120)
        {
            bounds.w = 120;
        }
    }

    Rect getBounds() { return bounds; }

private:
    static constexpr float WORLD_WIDTH = 1920.0f;
    static constexpr float DEFAULT_SPEED = 20.0f;
    inline static const Rect DEFAULT_BOUNDS{1920.0f / 2 - 250, 20, 300, 60};

    Rect bounds;
    Direction direction;
    float speed;
    std::shared_ptr<Graphics> graphics;
    float paddleAnimation = 0;
};
