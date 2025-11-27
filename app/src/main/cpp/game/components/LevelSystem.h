#pragma once

#include "../helpers/Event.h"
#include "../scene/ISceneComponent.h"
#include "../thirdparty/json.hpp"
#include "../helpers/AssetLoader.h"
#include "bricks/Brick.h"
#include <list>

using json = nlohmann::json;

namespace Breakout {

class LevelSystem : public ISceneComponent {
public:
    Event<Brick*> onBrickDestroy;
    Event<> onBrickDamage;
    Event<> onBrickDeflect;
    Event<> onBrickExplosion;
    Event<> onLevelStart;

    LevelSystem(AppContext* context)
            : appContext(context),
              levelBounds(0, 0, LEVEL_WIDTH, LEVEL_HEIGHT) {}

    void onAwake() override {
        createLevel(currentLevel);
    }

    void createLevel(int levelId) {
        std::string filename = "level" + std::to_string(levelId) + ".json";
        std::string jsonText = AssetLoader::load(appContext->assetManager, filename.c_str());

        if (jsonText.empty()) {
            return;
        }

        auto json = json::parse(jsonText);

        dynamicBricks.clear();
        allBricks.clear();
        destroyableBricks.clear();

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

    Rect getLevelBounds() { return levelBounds; }
    std::list<Brick> &getBricks() { return allBricks; }

    void explode(int gridX, int gridY) {
        int x = gridX;
        int y = gridY;

        if (Brick* right = GetBrick(x + 1, y))
            right->hit();

        if (Brick* left = GetBrick(x - 1, y))
            left->hit();

        if (Brick* top = GetBrick(x, y + 1))
            top->hit();

        if (Brick* bottom = GetBrick(x, y - 1))
            bottom->hit();
    }

    Brick* checkBrickCollision(const Rect& bounds) {
        int minX = static_cast<int>(bounds.x / Brick::BRICK_WIDTH);
        int maxX = static_cast<int>((bounds.x + bounds.w) / Brick::BRICK_WIDTH);
        int minY = static_cast<int>(bounds.y / Brick::BRICK_HEIGHT);
        int maxY = static_cast<int>((bounds.y + bounds.h) / Brick::BRICK_HEIGHT);

        if (minX < 0) minX = 0;
        if (maxX >= GRID_WIDTH)  maxX = GRID_WIDTH - 1;
        if (minY < 0) minY = 0;
        if (maxY >= GRID_HEIGHT) maxY = GRID_HEIGHT - 1;

        for (int y = minY; y <= maxY; ++y) {
            for (int x = minX; x <= maxX; ++x) {
                Brick* brick = staticBricks[x][y];
                if (brick && bounds.overlaps(brick->getBounds())) {
                    return brick;
                }
            }
        }

        for (Brick* b : dynamicBricks) {
            if (!b->getIsDestroyed() &&
                bounds.overlaps(b->getBounds())) {
                return b;
            }
        }

        return nullptr;
    }

    bool isBrickInMap(int gridX, int gridY) {
        return (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT);
    }

    Brick* GetBrick(int gridX, int gridY) {
        if(!isBrickInMap(gridX, gridY))
            return nullptr;

        for (Brick* dyn : dynamicBricks) {
            if (dyn->getGridX() == gridX &&
                dyn->getGridY() == gridY) {
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

    std::list<Brick> allBricks;
    std::list<Brick*> dynamicBricks;
    std::list<Brick*> destroyableBricks;
    std::array<std::array<Brick*, GRID_HEIGHT>, GRID_WIDTH> staticBricks{};
    AppContext* appContext;
    Rect levelBounds;
    int currentLevel = 1;

    void removeBrick(const Brick &brickRef) {
        // Remove from static grid if present
        if (brickRef.getIsDynamic()) {
            auto dynIt = std::remove_if(dynamicBricks.begin(), dynamicBricks.end(),
                                        [&](Brick* b) { return b == &brickRef; });
            dynamicBricks.erase(dynIt, dynamicBricks.end());
        } else {
            int gx = brickRef.getGridX();
            int gy = brickRef.getGridY();
            if (gx >= 0 && gx < GRID_WIDTH &&
                gy >= 0 && gy < GRID_HEIGHT) {
                staticBricks[gx][gy] = nullptr;
            }
        }

        if(brickRef.getIsDestructible()) {
            auto dynIt = std::remove_if(destroyableBricks.begin(), destroyableBricks.end(),
                                        [&](Brick* b) { return b == &brickRef; });
            destroyableBricks.erase(dynIt, destroyableBricks.end());
        }

        auto target = std::find_if(allBricks.begin(), allBricks.end(),
                                   [&](const Brick &b) { return &b == &brickRef; });
        if (target != allBricks.end()) {
            allBricks.erase(target);
        }
    }

    void createBrick(int gridX, int gridY, Brick::BrickType type) {
        allBricks.emplace_back(gridX, gridY, type);
         TODO:
        auto brick = &allBricks.back();
        if(brick->getIsDestructible()) {
            destroyableBricks.push_back(brick);
        }

        if(brick->getIsDynamic()) {
            dynamicBricks.push_back(brick);
        } else {
            staticBricks[gridX][gridY] = brick;
        }

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
};

}