#pragma once
#include "scene/ISceneComponent.h"
#include <android/log.h>
#include "Math.h"
#include "Brick.h"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Breakout", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Breakout", __VA_ARGS__)

class LevelManager : public ISceneComponent, public ISceneRender {
public:
    explicit LevelManager()
    {
        worldBounds = Rect(0, 0, 1080.0f, 1920.0f);
    }

    void onAwake() override {
        graphics = getComponent<Graphics>();

        const int rows = 6;
        const int cols = 14;
        const int brickWidth  = 100;
        const int brickHeight = 50;
        const int startX = 220;
        const int startY = 500;
        const int spacingX = 10;
        const int spacingY = 10;

        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                int x = startX + col * (brickWidth + spacingX);
                int y = startY + row * (brickHeight + spacingY);
                createBrick(x, y);
            }
        }
    }

    void onDestroy() override {

    }

    void onRender() override {
        graphics->drawRectangle(0, 0, 1920, 1080,
                                0, 0, 0, 1);

        for (auto& brick : bricks) {
            auto bounds = brick.getBounds();
            graphics->drawRectangle(bounds.x, bounds.y, bounds.w, bounds.h,1, 1, 1, 1);
        }
    }

    void createBrick(float x, float y) {
        bricks.emplace_back(x, y);
    }

    void removeBrick(const Brick& brickRef) {
        auto it = std::find_if(bricks.begin(), bricks.end(),
            [&](const Brick& b) {
            return &b == &brickRef;
        });

        if (it != bricks.end()) {
            bricks.erase(it);
        }
    }

    Rect getWorldBounds() {
        return worldBounds;
    }

    const std::vector<Brick>& getBricks() {
        return bricks;
    }

private:
    Rect worldBounds;
    std::shared_ptr<Graphics> graphics;
    std::vector<Brick> bricks;
};