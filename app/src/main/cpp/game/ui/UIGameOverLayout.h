#pragma once
#include "../GraphicsManager.h"

class UIGameOverLayout : public GraphicsManager {
public:
    UIGameOverLayout(AppContext* context) : GraphicsManager(context) {}

    void onAwake() override {
        resourceBackground = loadImage("background.png");
    }

    void onUpdate () override {
        drawImage(resourceBackground, 0, 0, 1920, 1080);

        graphicsAPI.flip();
    }


private:
    GLuint resourceBackground;
};