#pragma once

#include "../helpers/Math.h"
#include "../helpers/AppContext.h"
#include "../scene/ISceneComponent.h"
#include "../scene/ISceneComponentRender.h"

class UIRenderer : public GraphicsManager {
public:
    enum class TouchAction {
        Down = 0,
        Up = 1,
        Move = 2,
        Cancel = 3,
        PointerDown = 5,
        PointerUp = 6
    };

    UIRenderer(AppContext* context) : GraphicsManager(context) {}

    void onAwake() override {
        inputHandler = getComponent<UIInputHandler>();
    }

    void onRender() override {
        graphicsAPI.flip();
    }

    bool drawButton(GLuint textureId, float x, float y, float w, float h) {

        drawImage(textureId, x, y, w, h);

        if(inputHandler->isPressed())
        {
            auto touchPosition = inputHandler->getPosition();
            auto imageBounds = Rect(x, y, w, h);
            auto inputBounds = Rect(touchPosition.x, touchPosition.y, 1, 1);

            if(imageBounds.overlaps(inputBounds))
            {
                return true;
            }
        }

        return false;
    }

private:

    std::shared_ptr<UIInputHandler> inputHandler;
};