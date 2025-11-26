#pragma once

#include "helpers/Math.h"
#include "scene/ISceneComponent.h"
#include "helpers/SoundsAPI.h"

class SoundsManager : public ISceneComponent {
public:
    SoundsManager(AppContext* context) : soundsAPI(context) {}

    void onAwake() override {
        soundsAPI.initialize();

        auto paddle = getComponent<Paddle>();
        if (paddle) {
            paddle->onHit.subscribe([this]() {
                soundsAPI.play("paddle");
            });
        }

        auto laserShooter = getComponent<LaserShooter>();
        if (laserShooter) {
            laserShooter->onLaserShoot.subscribe([this]() {
                soundsAPI.play("laser");
            });
        }

        auto ballSystem = getComponent<BallSystem>();
        ballSystem->onHitWall.subscribe([this]() {
            soundsAPI.play("wall");
        });

        ballSystem->onExplosion.subscribe([this]() {
            soundsAPI.play("explosion");
        });

        auto powerUpManager = getComponent<PowerUpSystem>();
        powerUpManager->onPickup.subscribe([this]() {
            soundsAPI.play("powerup");
        });

        auto levelManager = getComponent<LevelSystem>();
        if (levelManager) {
            levelManager->onDestroyBrick.subscribe([this](const Brick& brick) {
                soundsAPI.play("brick");
            });
        }
    }

    void onDestroy() override {
        soundsAPI.destroy();
    }

private:
    SoundsAPI soundsAPI;
};