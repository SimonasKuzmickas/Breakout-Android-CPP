#pragma once

#include "helpers/Math.h"
#include "scene/ISceneComponent.h"
#include "LevelSystem.h"

class PlayerState : public ISceneComponent {
public:

    void onAwake() override {
        levelSystem = getComponent<LevelSystem>();
        ballSystem = getComponent<BallSystem>();

        auto levelSystem = getComponent<LevelSystem>();
        if (levelSystem) {
            levelSystem->onDestroyBrick.subscribe([this](const Brick& brick) {
                score += 50;
            });
        }

        auto ballSystem = getComponent<BallSystem>();
        if (ballSystem) {
            ballSystem->onLost.subscribe([this]() {
                lives--;
            });
        }
    }

    void onDestroy() override {

    }

    void onUpdate() override {

    }

    int getScore() {
        return score;
    }

    int getLives(){
        return lives;
    }

private:
    std::shared_ptr<LevelSystem> levelSystem;
    std::shared_ptr<BallSystem> ballSystem;

    int score = 0;
    int lives = 3;
};
