#pragma once

#include "helpers/Math.h"
#include "scene/ISceneComponent.h"
#include "helpers/SoundsAPI.h"

namespace Breakout {

class SoundsManager : public ISceneComponent {
public:
    SoundsManager(AppContext *context) : soundsAPI(context) {}

    Sound *soundPaddle;
    Sound *soundWall;
    Sound *soundBrick;
    Sound *soundPowerUp;
    Sound *soundExplosion;
    Sound *soundLaser;
    Sound *lifeLost;


    void onAwake() override {
        soundsAPI.initialize();

        soundPaddle = soundsAPI.include("paddlehit");
        soundWall = soundsAPI.include("wallhit");
        soundBrick = soundsAPI.include("brickdestroy");
        soundPowerUp = soundsAPI.include("powerup");
        soundExplosion = soundsAPI.include("explosion");
        soundLaser = soundsAPI.include("laser");
        lifeLost = soundsAPI.include("lifelost");

        auto paddle = getComponent<Paddle>();
        if (paddle) {
            paddle->onHit.addListener([this]() {
                soundPaddle->play();
            });
        }

        auto laserShooter = getComponent<LaserShooter>();
        if (laserShooter) {
            laserShooter->onLaserShoot.addListener([this]() {
                soundLaser->play();
            });
        }

        auto ballSystem = getComponent<BallSystem>();
        ballSystem->onHitWall.addListener([this]() {
            soundWall->play();
        });

        ballSystem->onLost.addListener([this]() {
            lifeLost->play();
        });

        ballSystem->onExplosion.addListener([this]() {
            soundExplosion->play();
        });

        auto powerUpManager = getComponent<PowerUpSystem>();
        powerUpManager->onPickup.addListener([this]() {
            soundPowerUp->play();
        });

        auto levelSystem = getComponent<LevelSystem>();
        if (levelSystem) {
            levelSystem->onDestroyBrick.addListener([this](const Brick &brick) {
                soundBrick->play();
            });
        }


    }

    void onUpdate() override {

    }

    void onDestroy() override {
        soundsAPI.destroy();
    }

private:
    SoundsAPI soundsAPI;
};

}