#pragma once
#include "scene/ISceneComponent.h"
#include "Math.h"
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Breakout", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Breakout", __VA_ARGS__)

class GameWorld : public ISceneComponent, public ISceneRender {
public:
    explicit GameWorld(AppContext* context)
    {
        int width = ANativeWindow_getWidth(context->window);
        int height = ANativeWindow_getHeight(context->window);

        worldBounds = Rect(0, 0, (float)width, (float)height);
    }

    void onAwake() override {
        graphics = blackboard->getComponent<Graphics>();
    }

    void onDestroy() override {

    }

    void onRender() override {
        graphics->drawRectangle(0, 0, 100, 100,
                                0, 0, 0, 1);
    }

    Rect getWorldBounds() {
        return worldBounds;
    }

private:
    Rect worldBounds;
    std::shared_ptr<Graphics> graphics;
};