#pragma once

#include "../helpers/Event.h"
#include "../scene/ISceneComponent.h"
#include "../thirdparty/json.hpp"
#include "../helpers/AssetLoader.h"
#include <list>

using json = nlohmann::json;

namespace Breakout {

class LevelSystem;

struct Brick {
public:
    enum class BrickType {
        NormalOrange, // 0
        HardPurple, // 1
        HardBrown, // 2
        DynamicBlue, // 3
        DynamicGreen, // 4
        ExplodingYellow, // 5
        StaticGray // 6
    };

    Event<> onDamaged;

    static constexpr float BRICK_WIDTH = 160.0f;
    static constexpr float BRICK_HEIGHT = 60.0f;

    Brick(int gridX, int gridY, BrickType type)
            : bounds{gridX * BRICK_WIDTH, gridY * BRICK_HEIGHT, BRICK_WIDTH, BRICK_HEIGHT},
              type(type),
              gridX(gridX),
              gridY(gridY),
              isDestroyed(false),
              damaged(false) {}

    const Rect &getBounds() const { return bounds; }
    BrickType getType() const { return type; }
    int getGridX() const { return gridX; }
    int getGridY() const { return gridY; }
    bool getIsDestroyed() { return isDestroyed; }
    bool getIsDynamic() const {
        return type == BrickType::DynamicBlue || type == BrickType::DynamicGreen;
    }
    bool getIsDamaged() const {
        return damaged;
    }

    void update() {
        switch (type) {
            case BrickType::DynamicBlue: {
                float amplitude = 80.0f;   // max offset in pixels
                float speed = 2.0f;    // oscillations per second

                // Compute offset using sine wave
                float t = GameTime::realtimeSinceStartup();
                float offsetX = amplitude * std::sin(speed * t);

                // Update bounds.x relative to grid position
                bounds.x = gridX * BRICK_WIDTH + offsetX;
                bounds.y = gridY * BRICK_HEIGHT;
                break;
            }

            case BrickType::DynamicGreen: {
                float amplitude = 40.0f;
                float speed     = 1.5f;

                float t = GameTime::realtimeSinceStartup();
                float offsetY = amplitude * std::sin(speed * t);

                bounds.x = gridX * BRICK_WIDTH;
                bounds.y = gridY * BRICK_HEIGHT + offsetY;
                break;
            }

            case BrickType::ExplodingYellow:

                break;
        }
    }

    void hit() {
        switch (type) {
            case BrickType::HardPurple:
            case BrickType::HardBrown:
                if(!damaged) {
                    damaged = true;
                    onDamaged.invoke();
                    return;
                }
                break;
        }

        isDestroyed = true;
    }

private:
    Rect bounds;
    int gridX, gridY;
    BrickType type;
    bool isDestroyed;
    bool damaged;
};

class LevelSystem : public ISceneComponent {
public:
    Event<Brick> onDestroyBrick;
    Event<> onDamageBrick;
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
        // TODO: clear array?

        for (auto& b : json["bricks"]) {
            int x = b["x"].get<int>();
            int y = b["y"].get<int>();
            int type = b["type"].get<int>();

            createBrick(x, y, static_cast<Brick::BrickType>(type));
        }

        onLevelStart.invoke();
    }

    void onUpdate() override {
        if (allBricks.empty()) {
            currentLevel = (currentLevel + 1) % TOTAL_LEVELS;
            createLevel(currentLevel);
        }

        for (auto &brick: allBricks) {
            brick.update();

            if(brick.getIsDestroyed()) {
                onDestroyBrick.invoke(brick);
                removeBrick(brick);
            }
        }
    }

    Rect getLevelBounds() { return levelBounds; }
    std::list<Brick> &getBricks() { return allBricks; }

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

        // check dynamic bricks collisions
        return nullptr;
    }

    bool isBrickInMap(int gridX, int gridY) {
        return (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT);
    }

    Brick* GetBrick(int gridX, int gridY) {
        if(!isBrickInMap(gridX, gridY))
            return nullptr;

        // get dynamic bricks

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
    std::array<std::array<Brick*, GRID_HEIGHT>, GRID_WIDTH> staticBricks{};
    AppContext* appContext;
    Rect levelBounds;
    int currentLevel = 1;

    void removeBrick(const Brick &brickRef) {

        // Remove from static grid if present
        if (!brickRef.getIsDynamic()) {
            int gx = brickRef.getGridX();
            int gy = brickRef.getGridY();
            if (gx >= 0 && gx < GRID_WIDTH &&
                gy >= 0 && gy < GRID_HEIGHT) {
                staticBricks[gx][gy] = nullptr;
            }
        } else {
            // Remove from dynamic list
            auto dynIt = std::remove_if(dynamicBricks.begin(), dynamicBricks.end(),
                                        [&](Brick* b) { return b == &brickRef; });
            dynamicBricks.erase(dynIt, dynamicBricks.end());
        }

        // Finally erase from master list
        auto target = std::find_if(allBricks.begin(), allBricks.end(),
                                   [&](const Brick &b) { return &b == &brickRef; });
        if (target != allBricks.end()) {
            allBricks.erase(target);
        }
    }

    void createBrick(int gridX, int gridY, Brick::BrickType type) {
        allBricks.emplace_back(gridX, gridY, type);

        auto brick = &allBricks.back();
        if(brick->getIsDynamic())
        {
            dynamicBricks.push_back(brick);
        } else {
            staticBricks[gridX][gridY] = brick;
        }

        brick->onDamaged.addListener([this]() {
            onDamageBrick.invoke();
        });
    }
};

}