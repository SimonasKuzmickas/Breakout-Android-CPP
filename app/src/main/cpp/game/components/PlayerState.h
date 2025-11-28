#pragma once

#include "../scene/ISceneComponent.h"
#include "LevelSystem.h"

namespace Breakout {

class PlayerState : public ISceneComponent {
public:
    Event<> onDeath;

    PlayerState(std::shared_ptr<LevelSystem> levelSystem, std::shared_ptr<BallSystem> ballSystem)
    : levelSystem(levelSystem), ballSystem(ballSystem)
    {}

    // --- Initialize listeners for level and ball events
    void onAwake() override {
        levelSystem->onBrickDestroy.addListener([this](const Brick* brick) {
            increaseScore(BRICK_SCORE_VALUE);
        });

        ballSystem->onLost.addListener([this]() {
            increaseLives(-1);
        });
    }

    // Modify player lives
    void increaseLives(int amount) {
        lives += amount;
        if(lives <= 0) {
            lives = 0;
            onDeath.invoke();
        }
    }

    void increaseScore(int amount) {
        score = std::max(0, score + amount);
    }

    int getScore() { return score; }
    int getLives() { return lives; }

private:
    std::shared_ptr<LevelSystem> levelSystem;
    std::shared_ptr<BallSystem> ballSystem;

    int score = 0;
    int lives = INITIAL_LIVES;

    static constexpr int INITIAL_LIVES = 3;
    static constexpr int BRICK_SCORE_VALUE = 5;
};

}