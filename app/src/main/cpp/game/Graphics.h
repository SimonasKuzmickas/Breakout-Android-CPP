#pragma once

#include "helpers/GraphicsAPI.h"

class Graphics : public ISceneComponent, public ISceneRender {
public:
    Graphics(AppContext* context) : graphicsAPI(context) {}

    GLuint resourceBackground;
    std::array<GLuint, 3> resourcePaddles;
    GLuint resourceBall;
    GLuint resourceBrick1;
    GLuint resourcePowerUp1;

    void onAwake() override {
        for (int i = 1; i <= 3; ++i) {
            std::string filename = "paddle" + std::to_string(i) + ".png";
            resourcePaddles[i - 1] = graphicsAPI.loadTextureFromAssets(filename.c_str());
        }
        resourceBackground = graphicsAPI.loadTextureFromAssets("background.png");
        resourceBall = graphicsAPI.loadTextureFromAssets("ball.png");
        resourceBrick1 = graphicsAPI.loadTextureFromAssets("brick1.png");
        resourcePowerUp1 = graphicsAPI.loadTextureFromAssets("powerup1.png");
    }

    void onDestroy() override {}

    void onRender() override {
        graphicsAPI.flip();
    }

    void drawImage(GLuint textureId, float x, float y, float w, float h, float r, float g, float b, float a) {
        graphicsAPI.drawTexture(textureId, x, y, w, h);
    }

private:
    GraphicsAPI graphicsAPI;
};