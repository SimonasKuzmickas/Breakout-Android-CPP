#pragma once

#include "../helpers/Event.h"
#include "../scene/ISceneComponent.h"

namespace Breakout {

struct Brick {
public:
    Brick(float x, float y, int type)
            : bounds{x, y, 160, 60},
            type(type){}

    [[nodiscard]] const Rect &getBounds() const {
        return bounds;
    }

private:
    Rect bounds;
    int type;
};

class LevelSystem : public ISceneComponent {
public:
    Event<Brick> onDestroyBrick;
    Event<> onlevelStart;

    explicit LevelSystem() {
        levelBounds = Rect(0, 0, 1920.0f, 1080.0f);
    }

    void onAwake() override {
        createLevel();
    }

    void createLevel() {
        const int rows = 18;
        const int cols = 14;
        const int brickWidth = 160;
        const int brickHeight = 60;

        for (int row = 10; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                createBrick(col, row, 0);
            }
        }

        onlevelStart.invoke();
    }

    void onUpdate() override {
        if (bricks.empty()) {
            createLevel();
        }
    }

    void createBrick(int gridX, int gridY, int type) {
        bricks.emplace_back(gridX * 160, gridY * 60, type);
    }

    void removeBrick(const Brick &brickRef) {
        auto target = std::find_if(bricks.begin(), bricks.end(),
                                   [&](const Brick &b) {
                                       return &b == &brickRef;
                                   });

        onDestroyBrick.invoke(brickRef);

        if (target != bricks.end()) {
            bricks.erase(target);
        }
    }

    Rect getLevelBounds() {
        return levelBounds;
    }

    std::vector<Brick> &getBricks() {
        return bricks;
    }

    Brick *checkBrickCollision(Rect bounds) {
        for (auto &brick: bricks) {
            if (bounds.overlaps(brick.getBounds())) {
                return &brick;
            }
        }
        return nullptr;
    }

private:
    Rect levelBounds;
    std::vector<Brick> bricks;
};

}