#pragma once

#include "../helpers/Math.h"
#include "../helpers/AppContext.h"
#include "../scene/ISceneComponent.h"
#include "../scene/ISceneComponentUpdate.h"

class UIInputHandler;
static UIInputHandler* g_inputHandler = nullptr;

class UIInputHandler : public ISceneComponent, public ISceneComponentUpdate {
public:
    enum class TouchAction {
        Down = 0,
        Up = 1,
        Move = 2,
        Cancel = 3,
        PointerDown = 5,
        PointerUp = 6
    };

    explicit UIInputHandler(AppContext* context)
    {
        int width = ANativeWindow_getWidth(context->window);
        int height = ANativeWindow_getHeight(context->window);

        windowBounds = Rect(0, 0, (float)width, (float)height);
    }

    void onAwake() override {
        g_inputHandler = this;
    }

    void onDestroy() override {

    }

    void onUpdate() override {

    }

    void onTouch(int pointerId, float x, float y, int action) {
        auto act = static_cast<TouchAction>(action);
        if (act == TouchAction::Down)
        {
            //position = Vector2(x, y);
        }
    }

private:
    Rect windowBounds;

};