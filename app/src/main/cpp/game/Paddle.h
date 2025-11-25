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

        // clamping to screen borders
        const float maxX = WORLD_WIDTH - bounds.w;
        bounds.x = std::clamp(bounds.x, 0.0f, maxX);
    }

    void onRender() override {
        paddleAnimation = std::fmod(paddleAnimation + 0.2f, 3.0f);

        graphics->drawImage(graphics->resourcePaddles[(int)paddleAnimation], bounds.x, bounds.y, bounds.w, bounds.h,
                                255, 0, 1, 1);
    }

    void onDestroy() override {

    }

    void move(Direction dir) {
        direction = dir;
    }

    Rect getBounds() { return bounds; }

private:
    static constexpr float WORLD_WIDTH = 1920.0f;
    static constexpr float DEFAULT_SPEED = 15.0f;
    inline static const Rect DEFAULT_BOUNDS{1920.0f / 2 - 250, 20, 300, 60};

    Rect bounds;
    Direction direction;
    float speed;
    std::shared_ptr<Graphics> graphics;
    float paddleAnimation = 0;
};
