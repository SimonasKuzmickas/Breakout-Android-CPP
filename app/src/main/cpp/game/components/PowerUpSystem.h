#pragma once

#include "../helpers/Math.h"
#include "../helpers/Event.h"
#include "../scene/ISceneComponent.h"
#include "Paddle.h"
#include "LaserShooter.h"
#include "BallSystem.h"
#include "PlayerState.h"

namespace Breakout {

struct PowerUp {
    static constexpr float WIDTH  = 120.0f;
    static constexpr float HEIGHT = 40.0f;

    enum class PowerUpType {
        None,
        MultiBall,
        FastBall,
        SlowBall,
        ExpandPaddle,
        ShrinkPaddle,
        Laser,
        StrongBall,
        ExplodeBall,
        ExtraLife,
        Score50,
        Score100,
        Score250,
        Score500
    };

    PowerUp(float x, float y, PowerUpType type)
            : bounds{x, y,  WIDTH, HEIGHT}, powerUpType{type} {}

    Rect bounds;
    PowerUpType powerUpType;
};

class PowerUpSystem : public ISceneComponent {
public:
    Event<> onPickup;

    void onAwake() override {
        levelSystem = getComponent<LevelSystem>();
        paddle = getComponent<Paddle>();
        laserShooter = getComponent<LaserShooter>();
        ballSystem = getComponent<BallSystem>();
        playerState = getComponent<PlayerState>();

        if (levelSystem) {
            levelSystem->onDestroyBrick.addListener([this](const Brick &brick) {
                int rnd = std::rand() % 100;
                if (rnd < DROP_CHANCE && powerUps.size() < MAX_ACTIVE_POWERUPS) {
                    createPowerUp(brick.getBounds().x + 10, brick.getBounds().y + 5,
                                  getRandomPowerUpType());
                }
            });

            levelSystem->onlevelStart.addListener([this]() {
                powerUps.clear();
            });

            ballSystem->onLost.addListener([this]() {
                powerUps.clear();
            });
        }
    }

    void onUpdate() override {
        for (auto &powerup: powerUps) {
            powerup.bounds.y -= POWERUP_SPEED * GameTime::deltaTime();

            if (paddle->getBounds().overlaps(powerup.bounds)) {
                ApplyEffect(powerup.powerUpType);
                onPickup.invoke();
                removePowerUp(powerup);
            } else if (!levelSystem->getLevelBounds().overlaps(powerup.bounds)) {
                removePowerUp(powerup);
            }
        }
    }

    void createPowerUp(float x, float y, PowerUp::PowerUpType powerUpType) {
        powerUps.emplace_back(x, y, powerUpType);
    }

    void removePowerUp(const PowerUp &powerUpRef) {
        auto target = std::find_if(powerUps.begin(), powerUps.end(),
                                   [&](const PowerUp &b) {
                                       return &b == &powerUpRef;
                                   });

        if (target != powerUps.end()) {
            powerUps.erase(target);
        }
    }

    void ApplyEffect(PowerUp::PowerUpType powerUpType) {
        switch (powerUpType) {
            case PowerUp::PowerUpType::MultiBall:
                for (const auto &ball: ballSystem->getBalls()) {
                    auto v = ball.velocity;

                    ballSystem->createBall(ball.bounds.x, ball.bounds.y, v.rotate(0.2f));
                    ballSystem->createBall(ball.bounds.x, ball.bounds.y, v.rotate(-0.2f));
                }
                break;

            case PowerUp::PowerUpType::StrongBall:
                ballSystem->setBallType(BallSystem::BallsType::Strong);
                break;

            case PowerUp::PowerUpType::ExplodeBall:
                ballSystem->setBallType(BallSystem::BallsType::Explode);
                break;

            case PowerUp::PowerUpType::Laser:
                laserShooter->setActive(true);
                break;

            case PowerUp::PowerUpType::ExpandPaddle:
                paddle->expand(150);
                break;

            case PowerUp::PowerUpType::ShrinkPaddle:
                paddle->expand(-150);
                break;

            case PowerUp::PowerUpType::FastBall:
                ballSystem->increaseGlobalSpeed(1.5f);
                break;

            case PowerUp::PowerUpType::SlowBall:
                ballSystem->increaseGlobalSpeed(0.75f);
                break;

            case PowerUp::PowerUpType::Score50:
                playerState->increaseScore(50);
                break;

            case PowerUp::PowerUpType::Score100:
                playerState->increaseScore(100);
                break;

            case PowerUp::PowerUpType::Score250:
                playerState->increaseScore(250);
                break;

            case PowerUp::PowerUpType::Score500:
                playerState->increaseScore(500);
                break;

            case PowerUp::PowerUpType::ExtraLife:
                playerState->increaseLives(1);
                break;

            default:
                break;
        }
    }

    const std::vector<PowerUp> &getPowerUps() { return powerUps; }

private:
    static constexpr float POWERUP_SPEED = 300.0f;
    static constexpr int DROP_CHANCE = 50;
    static constexpr int MAX_ACTIVE_POWERUPS = 5;

    std::vector<PowerUp> powerUps;
    std::shared_ptr<LevelSystem> levelSystem;
    std::shared_ptr<BallSystem> ballSystem;
    std::shared_ptr<Paddle> paddle;
    std::shared_ptr<LaserShooter> laserShooter;
    std::shared_ptr<PlayerState> playerState;

    PowerUp::PowerUpType getRandomPowerUpType() {
        static bool seeded = false;
        if (!seeded) {
            std::srand(static_cast<unsigned>(std::time(nullptr)));
            seeded = true;
        }
        int r = std::rand() % 14;
        return static_cast<PowerUp::PowerUpType>(r);
    }
};

}