#pragma once

#include <android/log.h>
#include "helpers/Math.h"
#include "helpers/Event.h"
#include "scene/ISceneComponent.h"
#include "Brick.h"
#include "Paddle.h"
#include "BallSystem.h"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Breakout", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Breakout", __VA_ARGS__)

struct PowerUp {
    enum class PowerUpType {
        MultiBall,
        None,
        ExtraLife,
        ExpandPaddle,
        ShrinkPaddle,
        SlowBall,
        FastBall,
        StickyPaddle,
        Laser,
        Shield
    };

    PowerUp(float x, float y, PowerUpType type)
            : bounds{x, y, 80, 40}, powerUpType{type} {}

    Rect bounds;
    PowerUpType powerUpType;
};

class PowerUpManager : public ISceneComponent, public ISceneRender, public ISceneUpdate {
public:
    Event<> onPickup;

    void onAwake() override {
        graphics = getComponent<Graphics>();
        levelManager = getComponent<LevelManager>();
        paddle = getComponent<Paddle>();
        ballSystem = getComponent<BallSystem>();

        if (levelManager) {
            levelManager->onDestroyBrick.subscribe([this](const Brick& brick) {
                createPowerUp(brick.getBounds().x + 10, brick.getBounds().y + 5, PowerUp::PowerUpType::MultiBall);
            });
        }
    }

    void onDestroy() override {

    }

    void onUpdate() override {
        for (auto& powerup : powerUps) {
            powerup.bounds.y -= 5;

            if(paddle->getBounds().overlaps(powerup.bounds))
            {
                removePowerUp(powerup);

                onPickup.invoke();

                for (const auto& ball : ballSystem->getBalls()) {
                    auto velocity = ball.velocity;
                    velocity.x = -velocity.x;
                    ballSystem->createBall(ball.bounds.x, ball.bounds.y, ball.bounds.w, velocity);
                }
            }
        }
    }

    void onRender() override {
        for (auto& powerup : powerUps) {
            auto bounds = powerup.bounds;
            graphics->drawImage(graphics->resourcePowerUps[0], bounds.x, bounds.y, bounds.w, bounds.h,1, 1, 1, 1);
        }
    }

    void createPowerUp(float x, float y, PowerUp::PowerUpType powerUpType) {
        powerUps.emplace_back(x, y, powerUpType);
    }

    void removePowerUp(const PowerUp& powerUpRef) {
        auto target = std::find_if(powerUps.begin(), powerUps.end(),
            [&](const PowerUp& b) {
                return &b == &powerUpRef;
            });

        if (target != powerUps.end()) {
            powerUps.erase(target);
        }
    }

private:
    std::vector<PowerUp> powerUps;
    std::shared_ptr<LevelManager> levelManager;
    std::shared_ptr<BallSystem> ballSystem;
    std::shared_ptr<Paddle> paddle;
    std::shared_ptr<Graphics> graphics;
};