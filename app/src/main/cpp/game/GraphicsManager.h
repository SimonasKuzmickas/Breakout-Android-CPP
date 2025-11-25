#pragma once

#include "helpers/GraphicsAPI.h"
#include "Paddle.h"
#include "BallSystem.h"
#include "PowerUpSystem.h"
#include "LaserShooter.h"

class GraphicsManager : public ISceneComponent, public ISceneRender {
public:
    GraphicsManager(AppContext* context) : graphicsAPI(context) {}

    void onAwake() override {
        paddle = getComponent<Paddle>();
        ballSystem = getComponent<BallSystem>();
        levelManager = getComponent<LevelSystem>();
        powerUpManager = getComponent<PowerUpSystem>();
        laserShooter = getComponent<LaserShooter>();

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
        resourceShooterLeft = graphicsAPI.loadTextureFromAssets("shooter_left.png");
        resourceShooterRight = graphicsAPI.loadTextureFromAssets("shooter_right.png");
    }

    void onDestroy() override {}

    void onRender() override {
        drawLevel();
        drawPaddle();
        drawBalls();
        drawPowerUps();
        drawLaserShooter();

        graphicsAPI.flip();
    }

    void drawImage(GLuint textureId, float x, float y, float w, float h) {
        graphicsAPI.drawTexture(textureId, x, y, w, h);
    }

private:
    GraphicsAPI graphicsAPI;

    std::shared_ptr<Paddle> paddle;
    std::shared_ptr<BallSystem> ballSystem;
    std::shared_ptr<LevelSystem> levelManager;
    std::shared_ptr<PowerUpSystem> powerUpManager;
    std::shared_ptr<LaserShooter> laserShooter;

    std::array<GLuint, 12> resourcePowerUps;
    std::array<GLuint, 3> resourcePaddles;
    std::array<GLuint, 3> resourceBalls;

    GLuint resourceShooterLeft;
    GLuint resourceShooterRight;
    GLuint resourcePaddleLeft;
    GLuint resourcePaddleRight;
    GLuint resourceBackground;
    GLuint resourceBrick1;

    float paddleAnimation = 0;

    void drawPowerUps() {
        for (auto& powerup : powerUpManager->getPowerUps()) {
            auto bounds = powerup.bounds;
            int index = static_cast<int>(powerup.powerUpType);
            drawImage(resourcePowerUps[index], bounds.x, bounds.y, bounds.w, bounds.h);
        }
    }

    void drawLaserShooter() {
        for (auto& laser : laserShooter->getLasers()) {
            auto bounds = laser.bounds;
            drawImage(resourceBalls[0], bounds.x, bounds.y, bounds.w, bounds.h);
        }

        if(laserShooter->getIsActive()) {
            auto bounds = paddle->getBounds();
            float cornerWidth  = 50.0f;

            drawImage(resourceShooterLeft, bounds.x, bounds.y, cornerWidth, bounds.h);
            drawImage(resourceShooterRight, bounds.x + bounds.w - cornerWidth, bounds.y, cornerWidth, bounds.h);
        }
    }

    void drawPaddle() {
        paddleAnimation = std::fmod(paddleAnimation + 0.2f, 3.0f);

        auto bounds = paddle->getBounds();
        float cornerWidth  = 50.0f;

        drawImage(resourcePaddles[(int)paddleAnimation], bounds.x + cornerWidth, bounds.y, bounds.w - cornerWidth * 2, bounds.h);
        drawImage(resourcePaddleLeft, bounds.x, bounds.y, cornerWidth, bounds.h);
        drawImage(resourcePaddleRight, bounds.x + bounds.w - cornerWidth, bounds.y, cornerWidth, bounds.h);
    }

    void drawBalls() {
        int index = static_cast<int>(ballSystem->getBallsType());

        for (auto& ball : ballSystem->getBalls()) {
            drawImage(resourceBalls[index], ball.bounds.x, ball.bounds.y, ball.bounds.w, ball.bounds.h);
        }
    }

    void drawLevel() {
        auto levelBounds = levelManager->getLevelBounds();
        drawImage(resourceBackground, levelBounds.x, levelBounds.y, levelBounds.w, levelBounds.h);

        for (auto& brick : levelManager->getBricks()) {
            auto bounds = brick.getBounds();
            drawImage(resourceBrick1, bounds.x, bounds.y, bounds.w, bounds.h);
        }
    }
};