#pragma once

#include "helpers/GraphicsAPI.h"
#include "Paddle.h"
#include "BallSystem.h"
#include "PowerUpSystem.h"
#include "LaserShooter.h"
#include "PlayerState.h"

class GraphicsManager : public ISceneComponent {
public:
    GraphicsManager(AppContext* context) : graphicsAPI(context) {}

    void onAwake() override {
        paddle = getComponent<Paddle>();
        ballSystem = getComponent<BallSystem>();
        levelSystem = getComponent<LevelSystem>();
        powerUpManager = getComponent<PowerUpSystem>();
        laserShooter = getComponent<LaserShooter>();
        playerState = getComponent<PlayerState>();

        for (int i = 0; i <= 9; ++i) {
            std::string filename = "num" + std::to_string(i) + ".png";
            numbers[i] = loadImage(filename.c_str());
        }

        for (int i = 1; i <= 12; ++i) {
            std::string filename = "powerup" + std::to_string(i) + ".png";
            resourcePowerUps[i - 1] = loadImage(filename.c_str());
        }

        for (int i = 1; i <= 3; ++i) {
            std::string filename = "paddle" + std::to_string(i) + ".png";
            resourcePaddles[i - 1] = loadImage(filename.c_str());
        }

        for (int i = 1; i <= 3; ++i) {
            std::string filename = "ball" + std::to_string(i) + ".png";
            resourceBalls[i - 1] = loadImage(filename.c_str());
        }

        resourceBackground = loadImage("background.png");
        resourceBrick1 = loadImage("brick1.png");
        resourcePaddleLeft = loadImage("paddleleft.png");
        resourcePaddleRight = loadImage("paddleright.png");
        resourceShooterLeft = loadImage("shooter_left.png");
        resourceShooterRight = loadImage("shooter_right.png");
        resourceLife = loadImage("life.png");
    }

    void onDestroy() override {
        graphicsAPI.shutdown();
    }

    void onUpdate() override {
        drawLevel();
        drawPaddle();
        drawBalls();
        drawPowerUps();
        drawLaserShooter();
        drawHUD();

        graphicsAPI.flip();
    }

    void drawImage(GLuint textureId, float x, float y, float w, float h) {
        graphicsAPI.drawTexture(textureId, x, y, w, h);
    }

    void drawNumber(int value, int x, int y, int digitWidth, int digitHeight, int spacing) {
        std::string str = std::to_string(value);

        // Total width of all digits + spacing
        int totalWidth = static_cast<int>(str.size()) * digitWidth +
                         (static_cast<int>(str.size()) - 1) * spacing;

        // If you want right-aligned, shift x by totalWidth
        // int xposStart = x - totalWidth;   // right-aligned
        // If you want left-aligned, just use x directly:
        int xposStart = x;

        for (size_t i = 0; i < str.size(); ++i) {
            int digit = str[i] - '0';
            int xpos = xposStart + static_cast<int>(i) * (digitWidth + spacing);

            drawImage(numbers[digit], xpos, y, digitWidth, digitHeight);
        }
    }

    GLuint loadImage(const char* path) {
        return graphicsAPI.loadTextureFromAssets(path);
    }

protected:
    GraphicsAPI graphicsAPI;

private:
    std::shared_ptr<Paddle> paddle;
    std::shared_ptr<BallSystem> ballSystem;
    std::shared_ptr<LevelSystem> levelSystem;
    std::shared_ptr<PowerUpSystem> powerUpManager;
    std::shared_ptr<LaserShooter> laserShooter;
    std::shared_ptr<PlayerState> playerState;

    std::array<GLuint, 12> resourcePowerUps;
    std::array<GLuint, 3> resourcePaddles;
    std::array<GLuint, 3> resourceBalls;
    std::array<GLuint, 10> numbers;

    GLuint resourceShooterLeft;
    GLuint resourceShooterRight;
    GLuint resourcePaddleLeft;
    GLuint resourcePaddleRight;
    GLuint resourceBackground;
    GLuint resourceBrick1;
    GLuint resourceLife;

    float paddleAnimation = 0;

protected:
    void drawHUD()
    {
        int score = playerState->getScore();
        int numDigits = (score == 0) ? 1 : static_cast<int>(std::log10(score)) + 1;

        int digitWidth = 60, digitHeight = 100, spacing = 20;
        int totalWidth = numDigits * (digitWidth + spacing) - spacing;

        int x = 1920 - 50 - totalWidth;
        int y = 1080 - 50 - digitHeight;

        drawNumber(score, x, y, digitWidth, digitHeight, spacing);

        int lives = playerState->getLives();
        for (int i = 0; i < lives; ++i) {
            drawImage(resourceLife, 50 + i * (50 + 10), 1080 - 50 - 50, 50, 50);
        }
    }

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
        auto levelBounds = levelSystem->getLevelBounds();
        drawImage(resourceBackground, levelBounds.x, levelBounds.y, levelBounds.w, levelBounds.h);

        for (auto& brick : levelSystem->getBricks()) {
            auto bounds = brick.getBounds();
            drawImage(resourceBrick1, bounds.x, bounds.y, bounds.w, bounds.h);
        }
    }
};