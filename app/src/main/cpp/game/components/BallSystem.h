#pragma once

#include "Paddle.h"
#include "LevelSystem.h"
#include "Ball/BallBlackboard.h"
#include "Ball/BallType.h"
#include "Ball/Ball.h"
#include "Ball/BallFactory.h"

namespace Breakout {

class BallSystem : public ISceneComponent {
public:
    Event<> onHitWall;
    Event<> onLostAllBalls;

    BallSystem(std::shared_ptr<Paddle>  paddle, std::shared_ptr<LevelSystem>  levelSystem) {
        ballBlackboard = new BallBlackboard(levelSystem, paddle);
    }

    void onAwake() override {
        ballBlackboard->levelSystem->onLevelStart.addListener([this]() { start(); });
        start();
    }

    void start() {
        balls.clear();

        createBall(
                    WORLD_WIDTH * 0.5f,
                    START_Y ,
                    randomBallStartingVelocity(SPEED_START),
                   BallType::Normal);

        ballBlackboard->paddle->start();
    }

    void onUpdate() override {
        for (auto &ball: balls) {
            ball->updateBallMovement();
            ball->updateBallCollisions();
        }

        if (balls.empty()) {
            onLostAllBalls.invoke();
            start();
        }

        for (const Ball* b : pendingRemovals) {
            auto target = std::find_if(balls.begin(), balls.end(),
                                       [&](const std::unique_ptr<Ball>& ptr) { return ptr.get() == b; });
            if (target != balls.end()) {
                balls.erase(target);
            }
        }
        pendingRemovals.clear();
    }

    void createBall(float x, float y, Vector2 v, BallType ballType) {
        balls.push_back(BallFactory::createBall(x, y, v, ballType, ballBlackboard));

        auto& ballPtr = balls.back();
        Ball* rawBall = ballPtr.get();

        rawBall->onHitWall.addListener([this]() {
            onHitWall.invoke();
        });

        rawBall->onOutOfScreen.addListener([this, rawBall]() {
            removeBall(*rawBall);
        });
    }

    void removeBall(const Ball& ballRef) {
        pendingRemovals.push_back(&ballRef);
    }

    void setBallType(BallType type) {
        std::vector<std::pair<Vector2, Rect>> originals;
        originals.reserve(balls.size());

        for (auto& ballPtr : balls) {
            originals.emplace_back(ballPtr->velocity, ballPtr->bounds);
        }

        // Clear old balls
        balls.clear();

        // Recreate balls of the new type
        for (auto& [vel, bounds] : originals) {
            createBall(bounds.x, bounds.y, vel, type);
        }
    }

    void increaseGlobalSpeed(float multiplier) {
        for (auto &ball: balls) {
            ball->speedMultiplier *= multiplier;
        }
    }

    BallBlackboard* getBlackboard() { return ballBlackboard; }
    const std::vector<std::unique_ptr<Ball>> &getBalls() const { return balls; }

private:
    static constexpr float SPEED_START = 600.0f;
    static constexpr float WORLD_WIDTH = 1920.0f;
    static constexpr float START_Y = 10.0f;

    std::vector<std::unique_ptr<Ball>> balls;
    std::vector<const Ball*> pendingRemovals;
    BallBlackboard* ballBlackboard;

    // Random starting velocity for new ball
    Vector2 randomBallStartingVelocity(float speed) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));

        // 45d to 135d
        float angle = M_PI / 4.0f +     (static_cast<float>(std::rand()) / RAND_MAX) * (M_PI / 2.0f);
        return {speed * std::cos(angle), speed * std::sin(angle)};
    }
};

}