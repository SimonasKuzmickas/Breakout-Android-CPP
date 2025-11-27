#pragma once

#include "../helpers/Event.h"
#include "../scene/ISceneComponent.h"
#include "../thirdparty/json.hpp"
using json = nlohmann::json;

namespace Breakout {

struct Brick {
public:
    static constexpr float BrickWidth = 160.0f;
    static constexpr float BrickHeight = 60.0f;

    Brick(float x, float y, int type)
            : bounds{x, y, BrickWidth, BrickHeight},
            type(type){}

    const Rect &getBounds() const { return bounds; }
    int getType() const { return type; }

private:
    Rect bounds;
    int type;
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
        std::string jsonText = loadAssetFile(appContext->assetManager, filename.c_str());

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
    }

    void createBrick(int gridX, int gridY, int type) {
        bricks.emplace_back(gridX * Brick::BrickWidth, gridY * Brick::BrickHeight, type);
    }

    void removeBrick(const Brick &brickRef) {
        auto target = std::find_if(bricks.begin(), bricks.end(),
            [&](const Brick &b) { return &b == &brickRef; });

        onDestroyBrick.invoke(brickRef);

        if (target != bricks.end()) {
            bricks.erase(target);
        }
    }

    Rect getLevelBounds() { return levelBounds; }
    std::vector<Brick> &getBricks() { return bricks; }

    Brick *checkBrickCollision(Rect bounds) {
        for (auto &brick: bricks) {
            if (bounds.overlaps(brick.getBounds())) {
                return &brick;
            }
        }
        return nullptr;
    }

private:
    static constexpr int TOTAL_LEVELS = 7;
    static constexpr int LEVEL_WIDTH = 1920;
    static constexpr int LEVEL_HEIGHT = 1080;

    Rect levelBounds;
    std::vector<Brick> bricks;
    AppContext* appContext;
    int currentLevel = 1;

    std::string loadAssetFile(AAssetManager* mgr, const char* filename) {
        AAsset* asset = AAssetManager_open(mgr, filename, AASSET_MODE_BUFFER);
        if (!asset) return {};

        size_t size = AAsset_getLength(asset);
        std::string buffer(size, '\0');
        AAsset_read(asset, buffer.data(), size);
        AAsset_close(asset);
        return buffer;
    }
};

}