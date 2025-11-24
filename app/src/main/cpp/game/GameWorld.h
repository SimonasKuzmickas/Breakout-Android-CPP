#pragma once
#include "Math.h"
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Breakout", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Breakout", __VA_ARGS__)

class GameWorld : public ISceneComponent {
public:
    explicit GameWorld(AppContext* context)
    {
        int width = ANativeWindow_getWidth(context->window);
        int height = ANativeWindow_getHeight(context->window);

        worldBounds = Rect(0, 0, (float)width, (float)height);
    }

    void onAwake() override {

    }

    void onDestroy() override {

    }

    Rect getWorldBounds() {
        return worldBounds;
    }

private:
    Rect worldBounds;
};