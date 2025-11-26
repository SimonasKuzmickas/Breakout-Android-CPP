#pragma once

#include "../helpers/Math.h"
#include "../helpers/AppContext.h"
#include "../scene/ISceneComponent.h"
#include "../scene/ISceneComponentRender.h"

class UIRenderer : public ISceneComponent, public ISceneComponentRender {
public:
    enum class TouchAction {
        Down = 0,
        Up = 1,
        Move = 2,
        Cancel = 3,
        PointerDown = 5,
        PointerUp = 6
    };

    explicit UIRenderer(AppContext* context) : graphicsAPI(context) {}

    void onAwake() override {

    }

    void onDestroy() override {

    }

    void onRender() override {
        graphicsAPI.flip();
    }

    GLuint loadImage(const char* path) {
        return graphicsAPI.loadTextureFromAssets("background.png");
    }

    void drawImage(GLuint textureId, float x, float y, float w, float h) {
        graphicsAPI.drawTexture(textureId, x, y, w, h);
    }

    bool drawButton(GLuint textureId, float x, float y, float w, float h) {
        graphicsAPI.drawTexture(textureId, x, y, w, h);

        return true;
    }

private:
    GraphicsAPI graphicsAPI;
};