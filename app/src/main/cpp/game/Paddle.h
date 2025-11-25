#pragma once
#include "Math.h"
#include "Event.h"
#include <algorithm>
#include <android/log.h>

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
        graphics->drawRectangle(bounds.x, bounds.y, bounds.w, bounds.h,
                                255, 0, 1, 1);
    }

    void onDestroy() override {

    }

    void hitBall()
    {
        onHit.invoke();
    }

    void move(Direction dir) {
        direction = dir;
    }

    Rect getBounds() { return bounds; }

private:
    static constexpr float WORLD_WIDTH = 1920.0f;
    static constexpr float DEFAULT_SPEED = 15.0f;
    inline static const Rect DEFAULT_BOUNDS{1920.0f / 2 - 150, 20, 300, 20};

    Rect bounds;
    Direction direction;
    float speed;
    std::shared_ptr<Graphics> graphics;
};
