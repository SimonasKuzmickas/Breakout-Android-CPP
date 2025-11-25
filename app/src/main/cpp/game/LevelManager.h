#pragma once

#include <android/log.h>
#include "helpers/Math.h"
#include "helpers/Event.h"
#include "scene/ISceneComponent.h"
#include "Brick.h"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Breakout", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Breakout", __VA_ARGS__)

class LevelManager : public ISceneComponent, public ISceneRender {
public:
    Event<> onDestroyBrick;

    explicit LevelManager()
    {
        levelBounds = Rect(0, 0, 1920.0f, 1080.0f);
    }

    void onAwake() override {
        graphics = getComponent<Graphics>();

        const int rows = 6;
        const int cols = 14;
        const int brickWidth  = 100;
        const int brickHeight = 50;
        const int startX = 220;
        const int startY = 600;
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
        graphics->drawImage(graphics->resourceBackground, levelBounds.x, levelBounds.y, levelBounds.w, levelBounds.h,
                                0, 0, 0, 1);

        for (auto& brick : bricks) {
            auto bounds = brick.getBounds();
            graphics->drawImage(graphics->resourceBrick1, bounds.x, bounds.y, bounds.w, bounds.h,1, 1, 1, 1);
        }
    }

    void createBrick(float x, float y) {
        bricks.emplace_back(x, y);
    }

    void removeBrick(const Brick& brickRef) {
        auto target = std::find_if(bricks.begin(), bricks.end(),
            [&](const Brick& b) {
            return &b == &brickRef;
        });

        if (target != bricks.end()) {
            bricks.erase(target);
        }

        onDestroyBrick.invoke();
    }

    Rect getLevelBounds() {
        return levelBounds;
    }

    const std::vector<Brick>& getBricks() {
        return bricks;
    }

private:
    Rect levelBounds;
    std::shared_ptr<Graphics> graphics;
    std::vector<Brick> bricks;
};