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
        None,
        MultiBall,
        FastBall,
        SlowBall,
        ExpandPaddle,
        ShrinkPaddle,
        Laser,
        StrongBall,
        FireBall,
        ExtraLife,
        Score50,
        Score100,
        Score250,
        Score500
    };

    PowerUp(float x, float y, PowerUpType type)
            : bounds{x, y, 120, 40}, powerUpType{type} {}

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
                int rnd = std::rand() % 10;
                if(rnd < 4) {
                    createPowerUp(brick.getBounds().x + 10, brick.getBounds().y + 5, getRandomPowerUpType());
                }
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
                ApplyEffect(powerup.powerUpType);
                onPickup.invoke();
                removePowerUp(powerup);
            }
        }
    }

    void onRender() override {
        for (auto& powerup : powerUps) {
            auto bounds = powerup.bounds;
            int index = static_cast<int>(powerup.powerUpType);
            graphics->drawImage(graphics->resourcePowerUps[index], bounds.x, bounds.y, bounds.w, bounds.h);
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

    void ApplyEffect(PowerUp::PowerUpType powerUpType)
    {
        switch (powerUpType) {
            case PowerUp::PowerUpType::MultiBall:
                for (const auto &ball: ballSystem->getBalls()) {
                    auto velocity = ball.velocity;
                    velocity.x = -velocity.x;
                    velocity.y = -velocity.y;
                    ballSystem->createBall(ball.bounds.x, ball.bounds.y, ball.bounds.w, velocity);
                }
                break;

            case PowerUp::PowerUpType::ExpandPaddle:
                paddle->expand(150);

            default:

                break;
        }
    }

private:
    std::vector<PowerUp> powerUps;
    std::shared_ptr<LevelManager> levelManager;
    std::shared_ptr<BallSystem> ballSystem;
    std::shared_ptr<Paddle> paddle;
    std::shared_ptr<Graphics> graphics;

    PowerUp::PowerUpType getRandomPowerUpType() {
        static bool seeded = false;
        if (!seeded) {
            std::srand(static_cast<unsigned>(std::time(nullptr)));
            seeded = true;
        }
        int r = std::rand() % 11; // 0..10
        return static_cast<PowerUp::PowerUpType>(r);
    }
};