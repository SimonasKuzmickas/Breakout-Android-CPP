#pragma once

#include "../helpers/Event.h"
#include "../scene/ISceneComponent.h"
#include "../thirdparty/json.hpp"
using json = nlohmann::json;

namespace Breakout {

struct Brick {
public:
    Brick(float x, float y, int type)
            : bounds{x, y, 160, 60},
            type(type){}

    const Rect &getBounds() const {
        return bounds;
    }

    int getType() const {
        return type;
    }

private:
    Rect bounds;
    int type;
};

class LevelSystem : public ISceneComponent {
public:
    Event<Brick> onDestroyBrick;
    Event<> onlevelStart;

    LevelSystem(AppContext *context) : appContext(context) {
        levelBounds = Rect(0, 0, 1920.0f, 1080.0f);
    }

    void onAwake() override {

        createLevel(currentLevel);
    }

    void createLevel(int levelId) {
        std::string filename = "level" + std::to_string(levelId) + ".json";
        std::string jsonText = loadAssetFile(appContext->assetManager, filename.c_str());

        json j = json::parse(jsonText);

        bricks.clear();

        for (auto& b : j["bricks"]) {
            int x = b["x"].get<int>();
            int y = b["y"].get<int>();
            createBrick(x, y, 0);
        }

        onlevelStart.invoke();
    }

    void onUpdate() override {
        if (bricks.empty()) {
            createLevel(currentLevel);
            currentLevel++;
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

    std::string loadAssetFile(AAssetManager* mgr, const char* filename) {
        AAsset* asset = AAssetManager_open(mgr, filename, AASSET_MODE_BUFFER);
        if (!asset) return {};

        size_t size = AAsset_getLength(asset);
        std::string buffer(size, '\0');
        AAsset_read(asset, buffer.data(), size);
        AAsset_close(asset);
        return buffer;
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
    AppContext* appContext;
    int currentLevel = 1;
};

}