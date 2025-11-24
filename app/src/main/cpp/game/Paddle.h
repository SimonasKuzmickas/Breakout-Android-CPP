#pragma once
#include "modules/IModule.h"
#include "Math.h"
#include <android/log.h>
#include "GameWorld.h"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Breakout", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Breakout", __VA_ARGS__)

class Paddle : public IModule {
public:

    void onStart() override {

        auto gameWorld = blackboard->getModule<GameWorld>();
        if (!gameWorld) {
            LOGI("GameWorld module not found!");
            return;
        }

        worldBounds = gameWorld->getWorldBounds();
        position = Vector2(worldBounds.width() / 2, worldBounds.height());
    }

    void onShutdown() override {

    }

private:
    Rect worldBounds;
    Vector2 position;
};