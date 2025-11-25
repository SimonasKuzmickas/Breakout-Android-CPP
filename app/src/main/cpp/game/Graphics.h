#pragma once

#include "helpers/GraphicsAPI.h"

class Graphics : public ISceneComponent, public ISceneRender {
public:
    Graphics(AppContext* context) : graphicsAPI(context) {}

    void onAwake() override {}
    void onDestroy() override {}

    void onRender() override {
        graphicsAPI.flip();
    }

    void drawRectangle(float x, float y, float w, float h, float r, float g, float b, float a) {
        graphicsAPI.drawRectangle(x,y, w, h, r, g, b, a);
    }

private:
    GraphicsAPI graphicsAPI;
};