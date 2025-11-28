#pragma once

#include "../helpers/Event.h"
#include "../scene/ISceneComponent.h"
#include "../thirdparty/json.hpp"
#include "../helpers/AssetLoader.h"
#include "Brick.h"
#include <list>

using json = nlohmann::json;

namespace Breakout {

class LevelSystem : public ISceneComponent {
public:
    // --- Events
    Event<Brick*> onBrickDestroy;
    Event<> onBrickDamage;
    Event<> onBrickDeflect;
    Event<> onBrickExplosion;
    Event<> onLevelStart;

    // --- Accessors ---
    Rect getLevelBounds() { return levelBounds; }
    std::list<Brick> &getBricks() { return allBricks; }

    LevelSystem(AppContext* context)
            : appContext(context),
              levelBounds(0, 0, LEVEL_WIDTH, LEVEL_HEIGHT) {}

    void onAwake() override {
        createLevel(currentLevel); // Load initial level
    }

    void onUpdate() override {
        if (destroyableBricks.empty()) {
            currentLevel = (currentLevel + 1) % TOTAL_LEVELS;
            createLevel(currentLevel);
        }

        for (auto &brick: allBricks) {
            brick.update();

            if(brick.getIsDestroyed()) {
                onBrickDestroy.invoke(&brick);
                removeBrick(brick);
            }
        }
    }

    void createLevel(int levelId) {
        std::string filename = "level" + std::to_string(levelId) + ".json";
        std::string jsonText = AssetLoader::load(appContext->assetManager, filename.c_str());

        if (jsonText.empty()) {
            return;
        }

        auto json = json::parse(jsonText);

        // Clear previous bricks
        dynamicBricks.clear();
        allBricks.clear();
        destroyableBricks.clear();

        // Create new bricks
        for (auto& column : staticBricks) {
            for (auto& brickPtr : column) {
                brickPtr = nullptr;
            }
        }

        for (auto& b : json["bricks"]) {
            int x = b["x"].get<int>();
            int y = b["y"].get<int>();
            int type = b["type"].get<int>();

            createBrick(x, y, static_cast<Brick::BrickType>(type));
        }

        onLevelStart.invoke();
    }

    void explode(int gridX, int gridY) {
        if (Brick* right  = GetBrick(gridX + 1, gridY)) right->hit();
        if (Brick* left   = GetBrick(gridX - 1, gridY)) left->hit();
        if (Brick* top    = GetBrick(gridX, gridY + 1)) top->hit();
        if (Brick* bottom = GetBrick(gridX, gridY - 1)) bottom->hit();

        onBrickExplosion.invoke();
    }

    // Returns the first brick colliding with a rectangle
    Brick* checkBrickCollision(const Rect& bounds) {
        int minX = std::max(0, static_cast<int>(bounds.x / Brick::BRICK_WIDTH));
        int maxX = std::min(GRID_WIDTH - 1, static_cast<int>((bounds.x + bounds.w) / Brick::BRICK_WIDTH));
        int minY = std::max(0, static_cast<int>(bounds.y / Brick::BRICK_HEIGHT));
        int maxY = std::min(GRID_HEIGHT - 1, static_cast<int>((bounds.y + bounds.h) / Brick::BRICK_HEIGHT));

        // Check static bricks
        for (int y = minY; y <= maxY; ++y) {
            for (int x = minX; x <= maxX; ++x) {
                Brick* brick = staticBricks[x][y];
                if (brick && bounds.overlaps(brick->getBounds())) {
                    return brick;
                }
            }
        }

        // Check dynamic bricks
        for (Brick* b : dynamicBricks) {
            if (!b->getIsDestroyed() && bounds.overlaps(b->getBounds())) {
                return b;
            }
        }

        return nullptr;
    }

    // Returns brick at grid position, dynamic bricks included
    Brick* GetBrick(int gridX, int gridY) {
        if(!isPositionInBrickMap(gridX, gridY))
            return nullptr;

        Rect brickBounds {
                gridX * Brick::BRICK_WIDTH,
                gridY * Brick::BRICK_HEIGHT,
                Brick::BRICK_WIDTH,
                Brick::BRICK_HEIGHT
        };

        for (Brick* dyn : dynamicBricks) {
            if (dyn->getBounds().overlaps(brickBounds)) {
                return dyn;
            }
        }

        return staticBricks[gridX][gridY];
    }

private:
    static constexpr int TOTAL_LEVELS = 7;
    static constexpr int LEVEL_WIDTH = 1920;
    static constexpr int LEVEL_HEIGHT = 1080;
    static constexpr int GRID_WIDTH = 12;
    static constexpr int GRID_HEIGHT = 18;

    std::list<Brick> allBricks;             // All bricks
    std::list<Brick*> dynamicBricks;        // Dynamic bricks
    std::list<Brick*> destroyableBricks;    // Destructible bricks
    std::array<std::array<Brick*, GRID_HEIGHT>, GRID_WIDTH> staticBricks{}; // Static brick grid

    AppContext* appContext;
    Rect levelBounds;
    int currentLevel = 1;

    void createBrick(int gridX, int gridY, Brick::BrickType type) {
        allBricks.emplace_back(gridX, gridY, type);

        auto brick = &allBricks.back();

        if(brick->getIsDynamic()) {
            dynamicBricks.push_back(brick);
        } else {
            staticBricks[gridX][gridY] = brick;
        }

        if(brick->getIsDestructible()) {
            destroyableBricks.push_back(brick);
        }

        // Setup brick events
        brick->onDamage.addListener([this]() {
            onBrickDamage.invoke();
        });

        brick->onExplode.addListener([this](Brick* b) {
            explode(b->getGridX(), b->getGridY());
            onBrickExplosion.invoke();
        });

        brick->onDeflect.addListener([this]() {
            onBrickDeflect.invoke();
        });
    }

    // Removes brick from all lists and grids
    void removeBrick(const Brick &brickRef) {
        if (brickRef.getIsDynamic()) {
            auto target = std::remove_if(dynamicBricks.begin(), dynamicBricks.end(),
                                        [&](Brick* b) { return b == &brickRef; });
            dynamicBricks.erase(target, dynamicBricks.end());
        } else {
            int gx = brickRef.getGridX();
            int gy = brickRef.getGridY();
            if (isPositionInBrickMap(gx, gy)) {
                staticBricks[gx][gy] = nullptr;
            }
        }

        if(brickRef.getIsDestructible()) {
            auto target = std::remove_if(destroyableBricks.begin(), destroyableBricks.end(),
                                        [&](Brick* b) { return b == &brickRef; });
            destroyableBricks.erase(target, destroyableBricks.end());
        }

        auto target = std::find_if(allBricks.begin(), allBricks.end(),
                                   [&](const Brick &b) { return &b == &brickRef; });
        if (target != allBricks.end()) {
            allBricks.erase(target);
        }
    }

    bool isPositionInBrickMap(int gridX, int gridY) {
        return (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT);
    }
};

}