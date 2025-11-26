#pragma once

#include "../helpers/Math.h"
#include "../scene/ISceneComponent.h"
#include "LevelSystem.h"

class PlayerState : public ISceneComponent {
public:
    Event<> onDeath;

    void onAwake() override {
        levelSystem = getComponent<LevelSystem>();
        ballSystem = getComponent<BallSystem>();

        auto levelSystem = getComponent<LevelSystem>();
        if (levelSystem) {
            levelSystem->onDestroyBrick.subscribe([this](const Brick& brick) {
                increaseScore(50);
            });
        }

        auto ballSystem = getComponent<BallSystem>();
        if (ballSystem) {
            ballSystem->onLost.subscribe([this]() {
                increaseLives(-1);
            });
        }
    }

    void onDestroy() override {

    }

    void onUpdate() override {

    }

    void increaseLives(int amount)
    {
        lives += amount;

        if(lives <= 0)
        {
            lives = 0;
            onDeath.invoke();
        }
    }

    void increaseScore(int amount)
    {
        score += amount;
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
