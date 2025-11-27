#pragma once

#include "helpers/GraphicsCore.h"
#include "components/Paddle.h"
#include "components/BallSystem.h"
#include "components/PowerUpSystem.h"
#include "components/LaserShooter.h"
#include "components/PlayerState.h"

namespace Breakout {

class GraphicsManager : public ISceneComponent {
public:
    GraphicsManager(AppContext *context) : graphicsAPI(context) {}

    void onAwake() override {
        paddle = getComponent<Paddle>();
        ballSystem = getComponent<BallSystem>();
        levelSystem = getComponent<LevelSystem>();
        powerUpManager = getComponent<PowerUpSystem>();
        laserShooter = getComponent<LaserShooter>();
        playerState = getComponent<PlayerState>();

        resourceBackground = loadImage("bg_space.png");
        resourceBrick1 = loadImage("brick1.png");
        resourcePaddleLeft = loadImage("tex_paddleleft.png");
        resourcePaddleRight = loadImage("tex_paddleright.png");
        resourceShooterLeft = loadImage("tex_shooter_left.png");
        resourceShooterRight = loadImage("tex_shooter_right.png");
        resourceLife = loadImage("tex_life.png");
        resourceNumbers = loadImage("tex_numbers.png");
        resourceBalls = loadImage("tex_balls.png");
        resourcePowerUps = loadImage("tex_powerups.png");
        resourcePaddle = loadImage("tex_paddle.png");
    }

    void onDestroy() override {
        graphicsAPI.shutdown();
    }

    void onUpdate() override {
        drawGame();

        drawHUD();

        graphicsAPI.flip();
    }

    void drawImage(GLuint textureId, float x, float y, float w, float h) {
        graphicsAPI.drawTexture(textureId, x, y, w, h);
    }

    void drawImageAnim(GLuint textureId,
                       float x, float y, float w, float h,
                       int frameId,
                       int frameWidth, int frameHeight,
                       int sheetWidth, int sheetHeight) {
        graphicsAPI.drawTextureAnim(textureId, x, y, w, h,
                                    frameId, frameWidth, frameHeight, sheetWidth, sheetHeight);
    }

    void drawNumber(int value, int x, int y, int digitWidth, int digitHeight, int spacing) {
        std::string str = std::to_string(value);

        int totalWidth = static_cast<int>(str.size()) * digitWidth +
                         (static_cast<int>(str.size()) - 1) * spacing;

        int xposStart = x;
        for (size_t i = 0; i < str.size(); ++i) {
            int digit = str[i] - '0';
            int xpos = xposStart + static_cast<int>(i) * (digitWidth + spacing);

            drawImageAnim(resourceNumbers, xpos, y, digitWidth, digitHeight, digit, 7, 10, 70, 10);
        }
    }

    GLuint loadImage(const char *path) {
        return graphicsAPI.loadTextureFromAssets(path);
    }

protected:
    GraphicsCore graphicsAPI;

private:
    std::shared_ptr<Paddle> paddle;
    std::shared_ptr<BallSystem> ballSystem;
    std::shared_ptr<LevelSystem> levelSystem;
    std::shared_ptr<PowerUpSystem> powerUpManager;
    std::shared_ptr<LaserShooter> laserShooter;
    std::shared_ptr<PlayerState> playerState;

    GLuint resourceShooterLeft;
    GLuint resourceShooterRight;
    GLuint resourcePaddleLeft;
    GLuint resourcePaddleRight;
    GLuint resourceBackground;
    GLuint resourceBrick1;
    GLuint resourceLife;
    GLuint resourceNumbers;
    GLuint resourceBalls;
    GLuint resourcePowerUps;
    GLuint resourcePaddle;

    float paddleAnimation = 0;

protected:
    void drawGame() {
        drawBackground();

        graphicsAPI.setColor(0, 0, 0, 0.5f);
        graphicsAPI.setOffset(20, -20);
        // draw shadows

        drawLevel();
        drawPaddle();
        drawBalls();
        drawPowerUps();
        drawLaserShooter();

        graphicsAPI.setColor(1, 1, 1, 1);
        graphicsAPI.setOffset(0, 0);

        drawLevel();
        drawPaddle();
        drawBalls();
        drawPowerUps();
        drawLaserShooter();
    }

    void drawHUD() {
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
        for (auto &powerup: powerUpManager->getPowerUps()) {
            auto bounds = powerup.bounds;
            int index = static_cast<int>(powerup.powerUpType);
            drawImageAnim(resourcePowerUps, bounds.x, bounds.y, bounds.w, bounds.h,
                          index, 250, 68, 250, 952);
        }
    }

    void drawLaserShooter() {
        for (auto &laser: laserShooter->getLasers()) {
            auto bounds = laser.bounds;
            drawImageAnim(resourceBalls, bounds.x, bounds.y, bounds.w, bounds.h,
                          0, 50, 50, 150, 50);
        }

        if (laserShooter->getIsActive()) {
            auto bounds = paddle->getBounds();
            float cornerWidth = 50.0f;

            drawImage(resourceShooterLeft, bounds.x, bounds.y, cornerWidth, bounds.h);
            drawImage(resourceShooterRight, bounds.x + bounds.w - cornerWidth, bounds.y,
                      cornerWidth, bounds.h);
        }
    }

    void drawPaddle() {
        paddleAnimation = std::fmod(paddleAnimation + 0.2f, 3.0f);

        auto bounds = paddle->getBounds();
        float cornerWidth = 50.0f;

        drawImageAnim(resourcePaddle, bounds.x + cornerWidth, bounds.y,
                  bounds.w - cornerWidth * 2, bounds.h,
                      (int) paddleAnimation, 150, 66, 450, 66);
        drawImage(resourcePaddleLeft, bounds.x, bounds.y, cornerWidth, bounds.h);
        drawImage(resourcePaddleRight, bounds.x + bounds.w - cornerWidth, bounds.y, cornerWidth,
                  bounds.h);
    }

    void drawBalls() {
        int index = static_cast<int>(ballSystem->getBallsType());

        for (auto &ball: ballSystem->getBalls()) {
            drawImageAnim(resourceBalls, ball.bounds.x, ball.bounds.y, ball.bounds.w, ball.bounds.h,
                          index, 50, 50, 150, 50);
        }
    }

    void drawBackground() {
        auto levelBounds = levelSystem->getLevelBounds();
        drawImage(resourceBackground, levelBounds.x, levelBounds.y, levelBounds.w,
                  levelBounds.h);
    }

    void drawLevel() {
        for (auto &brick: levelSystem->getBricks()) {
            auto bounds = brick.getBounds();
            drawImage(resourceBrick1, bounds.x, bounds.y, bounds.w, bounds.h);
        }
    }
};

}