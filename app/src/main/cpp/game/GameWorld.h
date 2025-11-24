#pragma once
#include "modules/IModule.h"
#include "Math.h"
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Breakout", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Breakout", __VA_ARGS__)

class GameWorld : public IModule {
public:
    explicit GameWorld(AppContext* context)
    {
        int width = ANativeWindow_getWidth(context->window);
        int height = ANativeWindow_getHeight(context->window);

        worldBounds = Rect(0, 0, width, height);
        LOGI("Game World Start: %d x %d", worldBounds.width(), worldBounds.height());
      //  LOGI("Game World Start!" + context->window.);
    }

    void onStart() override   {

    }

    void onShutdown() override{

    }

    Rect getWorldBounds() {
        return worldBounds;
    }

private:
    Rect worldBounds;
};