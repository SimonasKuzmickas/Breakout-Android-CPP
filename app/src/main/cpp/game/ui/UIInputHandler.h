#pragma once

#include "../helpers/Math.h"
#include "../helpers/AppContext.h"
#include "../scene/ISceneComponent.h"
#include "../scene/ISceneComponentUpdate.h"

class UIInputHandler;
static UIInputHandler* g_inputHandler = nullptr;

class UIInputHandler : public ISceneComponent {
public:
    enum class TouchAction {
        Down = 0,
        Up = 1,
        Move = 2,
        Cancel = 3,
        PointerDown = 5,
        PointerUp = 6
    };

    explicit UIInputHandler(AppContext *context) {
        int width = ANativeWindow_getWidth(context->window);
        int height = ANativeWindow_getHeight(context->window);

        windowBounds = Rect(0, 0, (float) width, (float) height);
    }

    void onAwake() override {
        g_inputHandler = this;
    }

    void onDestroy() override {
        g_inputHandler = nullptr;
    }

    Vector2 getPosition() {
        return position;
    }

    bool isPressed()
    {
        bool result = pressed;
        pressed = false;

        return result;
    }

    void onTouch(float x, float y, int action) {
        auto actionEnum = static_cast<TouchAction>(action);

        pressed = actionEnum == TouchAction::Down;
        position = convertMouseToVirtual(x, y, windowBounds.w, windowBounds.h);
    }

private:
    Rect windowBounds;
    Vector2 position = {0, 0};
    bool pressed = false;

    Vector2 convertMouseToVirtual(int mouseX, int mouseY,
                                   int screenWidth, int screenHeight,
                                   float virtualWidth = 1920.0f,
                                   float virtualHeight = 1080.0f)
    {
        float targetAspect = virtualWidth / virtualHeight;
        float screenAspect = float(screenWidth) / screenHeight;

        int vpW, vpH, vpX, vpY;
        if (screenAspect > targetAspect) {
            vpH = screenHeight;
            vpW = int(vpH * targetAspect);
            vpX = (screenWidth - vpW) / 2; vpY = 0;
        } else {
            vpW = screenWidth;
            vpH = int(vpW / targetAspect);
            vpX = 0; vpY = (screenHeight - vpH) / 2;
        }

        if (mouseX < vpX || mouseX >= vpX + vpW ||
            mouseY < vpY || mouseY >= vpY + vpH)
            return {-1.0f, -1.0f};

        float normX = (mouseX - vpX) / float(vpW);
        float normY = 1.0f - (mouseY - vpY) / float(vpH);

        return {normX * virtualWidth, normY * virtualHeight};
    }
};