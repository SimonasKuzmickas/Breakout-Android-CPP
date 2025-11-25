#pragma once

#include "helpers/GraphicsAPI.h"

class Graphics : public ISceneComponent, public ISceneRender {
public:
    Graphics(AppContext* context) : graphicsAPI(context) {}

    std::array<GLuint, 12> resourcePowerUps;
    std::array<GLuint, 3> resourcePaddles;
    std::array<GLuint, 3> resourceBalls;
    GLuint resourcePaddleLeft;
    GLuint resourcePaddleRight;
    GLuint resourceBackground;
    GLuint resourceBrick1;

    void onAwake() override {
        for (int i = 1; i <= 12; ++i) {
            std::string filename = "powerup" + std::to_string(i) + ".png";
            resourcePowerUps[i - 1] = graphicsAPI.loadTextureFromAssets(filename.c_str());
        }

        for (int i = 1; i <= 3; ++i) {
            std::string filename = "paddle" + std::to_string(i) + ".png";
            resourcePaddles[i - 1] = graphicsAPI.loadTextureFromAssets(filename.c_str());
        }

        for (int i = 1; i <= 3; ++i) {
            std::string filename = "ball" + std::to_string(i) + ".png";
            resourceBalls[i - 1] = graphicsAPI.loadTextureFromAssets(filename.c_str());
        }

        resourceBackground = graphicsAPI.loadTextureFromAssets("background.png");
        resourceBrick1 = graphicsAPI.loadTextureFromAssets("brick1.png");
        resourcePaddleLeft = graphicsAPI.loadTextureFromAssets("paddleleft.png");
        resourcePaddleRight = graphicsAPI.loadTextureFromAssets("paddleright.png");
    }

    void onDestroy() override {}

    void onRender() override {
        graphicsAPI.flip();
    }

    void drawImage(GLuint textureId, float x, float y, float w, float h) {
        graphicsAPI.drawTexture(textureId, x, y, w, h);
    }

private:
    GraphicsAPI graphicsAPI;
};