#pragma once

#include "scene/ISceneComponent.h"
#include "helpers/SoundsCore.h"

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
    Sound *soundLifeLost;
    Sound *soundDeflect;

    void onAwake() override {
        soundsAPI.initialize();

        soundPaddle = soundsAPI.loadSound("sfx_paddlehit");
        soundWall = soundsAPI.loadSound("sfx_wallhit");
        soundBrick = soundsAPI.loadSound("sfx_brickdestroy");
        soundPowerUp = soundsAPI.loadSound("sfx_powerup");
        soundExplosion = soundsAPI.loadSound("sfx_explosion");
        soundLaser = soundsAPI.loadSound("sfx_laser");
        soundLifeLost = soundsAPI.loadSound("sfx_lifelost");
        soundDeflect = soundsAPI.loadSound("sfx_deflect");

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
            soundLifeLost->play();
        });

        auto powerUpManager = getComponent<PowerUpSystem>();
        powerUpManager->onPickup.addListener([this]() {
            soundPowerUp->play();
        });

        auto levelSystem = getComponent<LevelSystem>();
        if (levelSystem) {
            levelSystem->onBrickDestroy.addListener([this](const Brick* brick) {
                soundBrick->play();
            });

            levelSystem->onBrickDamage.addListener([this]() {
                soundBrick->play();
            });

            levelSystem->onBrickExplosion.addListener([this]() {
                soundExplosion->play();
            });

            levelSystem->onBrickDeflect.addListener([this]() {
                soundDeflect->play();
            });
        }
    }

    void onDestroy() override {
        soundsAPI.destroy();
    }

private:
    SoundsCore soundsAPI;
};

}