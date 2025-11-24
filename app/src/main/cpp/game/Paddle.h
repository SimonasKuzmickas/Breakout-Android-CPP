#pragma once
#include "modules/IModule.h"
#include "Math.h"

class Paddle : public IModule, public IModuleRender {
public:

    void onStart() override {
        graphics = blackboard->getModule<Graphics>();

        auto gameWorld = blackboard->getModule<GameWorld>();
        worldBounds = gameWorld->getWorldBounds();
        position = Rect(40, 1, 20, 5);
    }

    void onRender() override {
       graphics->drawRectangle(position.x, position.y, position.w, position.h, 255, 0, 1, 1);
    }

    void onShutdown() override {

    }

private:
    Rect worldBounds;
    Rect position;
    std::shared_ptr<Graphics> graphics;
};

//#include <android/log.h>
//
//#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Breakout", __VA_ARGS__)
//#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Breakout", __VA_ARGS__)
