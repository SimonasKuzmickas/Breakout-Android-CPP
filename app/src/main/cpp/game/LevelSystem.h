#pragma once

#include "helpers/Math.h"
#include "helpers/Event.h"
#include "scene/ISceneComponent.h"

struct Brick {
public:
    Brick(float x, float y)
            : bounds{x, y, 100, 50} {}

    [[nodiscard]] const Rect& getBounds() const {
        return bounds;
    }
private:
    Rect bounds;
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

    void onDestroy() override {

    }

    void onUpdate() override {

    }

    void createLevel()
    {
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

        onlevelStart.invoke();
    }

    void createBrick(float x, float y) {
        bricks.emplace_back(x, y);
    }

    void removeBrick(const Brick& brickRef) {
        auto target = std::find_if(bricks.begin(), bricks.end(),
           [&](const Brick& b) {
               return &b == &brickRef;
           });

        onDestroyBrick.invoke(brickRef);

        if (target != bricks.end()) {
            bricks.erase(target);
        }

        if(bricks.empty()) {
            createLevel();
        }
    }

    Rect getLevelBounds() {
        return levelBounds;
    }

    std::vector<Brick>& getBricks() {
        return bricks;
    }

    Brick* checkBrickCollision(Rect bounds)
    {
        for (auto& brick : bricks) {
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