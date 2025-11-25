#pragma once

#include <android/log.h>
#include "helpers/Math.h"
#include "helpers/Event.h"
#include "scene/ISceneComponent.h"
#include "Brick.h"
#include "Paddle.h"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Breakout", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Breakout", __VA_ARGS__)

struct PowerUp {
    PowerUp(float x, float y)
            : bounds{x, y, 80, 40} {}

    Rect bounds;
};


class PowerUpManager : public ISceneComponent, public ISceneRender, public ISceneUpdate {
public:
    Event<> onPickup;

    enum class PowerUpType {
        None,
        ExtraLife,
        ExpandPaddle,
        ShrinkPaddle,
        MultiBall,
        SlowBall,
        FastBall,
        StickyPaddle,
        Laser,
        Shield
    };

    void onAwake() override {
        graphics = getComponent<Graphics>();
        levelManager = getComponent<LevelManager>();
        paddle = getComponent<Paddle>();

        if (levelManager) {
            levelManager->onDestroyBrick.subscribe([this](const Brick& brick) {
                createPowerUp(brick.getBounds().x + 10, brick.getBounds().y + 5);
            });
        }
    }

    void onDestroy() override {

    }

    void onUpdate() override {
        for (auto& powerup : powerups) {
            powerup.bounds.y -= 5;

            if(paddle->getBounds().overlaps(powerup.bounds))
            {
                removePowerUp(powerup);

                onPickup.invoke();
            }
        }
    }

    void onRender() override {
        for (auto& powerup : powerups) {
            auto bounds = powerup.bounds;
            graphics->drawImage(graphics->resourcePowerUp1, bounds.x, bounds.y, bounds.w, bounds.h,1, 1, 1, 1);
        }
    }

    void createPowerUp(float x, float y) {
        powerups.emplace_back(x, y);
    }

    void removePowerUp(const PowerUp& powerUpRef) {
        auto target = std::find_if(powerups.begin(), powerups.end(),
                                   [&](const PowerUp& b) {
                                       return &b == &powerUpRef;
                                   });

        if (target != powerups.end()) {
            powerups.erase(target);
        }
    }

private:
    std::vector<PowerUp> powerups;
    std::shared_ptr<LevelManager> levelManager;
    std::shared_ptr<Paddle> paddle;
    std::shared_ptr<Graphics> graphics;
};