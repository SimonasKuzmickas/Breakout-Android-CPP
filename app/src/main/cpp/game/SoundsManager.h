#pragma once

#include <android/log.h>
#include "helpers/Math.h"
#include "Brick.h"
#include "scene/ISceneComponent.h"
#include "helpers/SoundsAPI.h"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Breakout", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Breakout", __VA_ARGS__)

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

        auto ballSystem = getComponent<BallSystem>();
        ballSystem->onHitWall.subscribe([this]() {
            soundsAPI.play("wall");
        });

        auto levelManager = getComponent<LevelManager>();
        if (levelManager) {
            levelManager->onDestroyBrick.subscribe([this]() {
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