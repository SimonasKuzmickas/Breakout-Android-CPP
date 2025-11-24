#pragma once
#include "scene/ISceneComponent.h"
#include "Math.h"

class Paddle : public ISceneComponent, public ISceneRender {
public:

    void onAwake() override {
        graphics = blackboard->getComponent<Graphics>();
        position = Rect(40, 1, 20, 5);
    }

    void onRender() override {
       graphics->drawRectangle(position.x, position.y, position.w, position.h, 255, 0, 1, 1);
    }

    void onDestroy() override {

    }

private:
    Rect position;
    std::shared_ptr<Graphics> graphics;
};
