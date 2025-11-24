#pragma once
#include "scene/ISceneComponent.h"
#include "Math.h"
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Breakout", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Breakout", __VA_ARGS__)

class LevelManager : public ISceneComponent, public ISceneRender {
public:
    explicit LevelManager()
    {
        worldBounds = Rect(0, 0, 1080.0f, 1920.0f);
    }

    void onAwake() override {
        graphics = blackboard->getComponent<Graphics>();
    }

    void onDestroy() override {

    }

    void onRender() override {
        graphics->drawRectangle(0, 0, 1920, 1080,
                                0, 0, 0, 1);
    }

    Rect getWorldBounds() {
        return worldBounds;
    }

private:
    Rect worldBounds;
    std::shared_ptr<Graphics> graphics;
};