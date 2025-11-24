#pragma once
#include "scene/ISceneComponent.h"
#include "Math.h"
#include <algorithm>

class Paddle : public ISceneComponent, public ISceneRender, public ISceneUpdate {
public:

    void onAwake() override {
        graphics = blackboard->getComponent<Graphics>();
        position = Rect(40, 1, 20, 5);
        speed = 0.75f;
    }

    void onRender() override {
       graphics->drawRectangle(position.x, position.y, position.w, position.h, 255, 0, 1, 1);
    }

    void onUpdate() override {
        position.x += direction * speed;

        float maxX = 100.0f - position.w;
        position.x = std::clamp(position.x, 0.0f, maxX);
    }

    void onDestroy() override {

    }
    
    // TODO: direction should be enum
    void move(int dir)
    {
        direction = (float)std::clamp(dir, -1, 1);
    }

private:
    Rect position;
    float direction;
    float speed;
    std::shared_ptr<Graphics> graphics;
};
