#pragma once
#include "scene/ISceneComponent.h"
#include "Math.h"

class Paddle : public ISceneComponent, public ISceneRender {
public:

    void onAwake() override {
        graphics = blackboard->getComponent<Graphics>();

        auto gameWorld = blackboard->getComponent<GameWorld>();
        worldBounds = gameWorld->getWorldBounds();
        position = Rect(40, 1, 20, 5);
    }

    void onRender() override {
       graphics->drawRectangle(position.x, position.y, position.w, position.h, 255, 0, 1, 1);
    }

    void onDestroy() override {

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
