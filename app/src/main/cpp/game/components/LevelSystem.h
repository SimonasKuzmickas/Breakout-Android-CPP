#pragma once

#include "../helpers/Event.h"
#include "../scene/ISceneComponent.h"
#include "../thirdparty/json.hpp"
#include "../helpers/AssetLoader.h"
#include <list>

using json = nlohmann::json;

namespace Breakout {

struct Brick {
public:
    static constexpr float BRICK_WIDTH = 160.0f;
    static constexpr float BRICK_HEIGHT = 60.0f;

    Brick(int gridX, int gridY, int type)
            : bounds{gridX * BRICK_WIDTH, gridY * BRICK_HEIGHT, BRICK_WIDTH, BRICK_HEIGHT},
            type(type),
            gridX(gridX),
            gridY(gridY),
            isDestroyed(false){}

    const Rect &getBounds() const { return bounds; }
    int getType() const { return type; }
    int getGridX() { return gridX; }
    int getGridY() { return gridY; }
    bool getIsDestroyed() { return isDestroyed; }

    void update()
    {

    }

    void destroy() {
        isDestroyed = true;
    }

private:
    Rect bounds;
    int gridX, gridY;
    int type;
    bool isDestroyed;
};

class LevelSystem : public ISceneComponent {
public:
    Event<Brick> onDestroyBrick;
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

        bricks.clear();

        for (auto& b : json["bricks"]) {
            int x = b["x"].get<int>();
            int y = b["y"].get<int>();
            int type = b["type"].get<int>();
            createBrick(x, y, type);
        }

        onLevelStart.invoke();
    }

    void onUpdate() override {
        if (bricks.empty()) {
            currentLevel = (currentLevel + 1) % TOTAL_LEVELS;
            createLevel(currentLevel);
        }

        for (auto &brick: bricks) {
            brick.update();

            if(brick.getIsDestroyed()) {
                removeBrick(brick);
            }
        }
    }

    Rect getLevelBounds() { return levelBounds; }
    std::list<Brick> &getBricks() { return bricks; }

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
                Brick* brick = bricksMap[x][y];
                if (brick && bounds.overlaps(brick->getBounds())) {
                    return brick;
                }
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

        return bricksMap[gridX][gridY];
    }

private:
    static constexpr int TOTAL_LEVELS = 7;
    static constexpr int LEVEL_WIDTH = 1920;
    static constexpr int LEVEL_HEIGHT = 1080;
    static constexpr int GRID_WIDTH = 12;
    static constexpr int GRID_HEIGHT = 18;

    Rect levelBounds;
    std::list<Brick> bricks;
    std::array<std::array<Brick*, GRID_HEIGHT>, GRID_WIDTH> bricksMap{};
    AppContext* appContext;
    int currentLevel = 1;

    void removeBrick(const Brick &brickRef) {
        auto target = std::find_if(bricks.begin(), bricks.end(),
                                   [&](const Brick &b) { return &b == &brickRef; });

        onDestroyBrick.invoke(brickRef);

        if (target != bricks.end()) {
            bricksMap[target->getGridX()][target->getGridY()] = nullptr;
            bricks.erase(target);
        }
    }

    void createBrick(int gridX, int gridY, int type) {
        bricks.emplace_back(gridX, gridY, type);

        bricksMap[gridX][gridY] = &bricks.back();
    }
};

}