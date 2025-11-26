#pragma once

#include "helpers/Math.h"
#include "helpers/Event.h"
#include "scene/ISceneComponent.h"
#include "Paddle.h"
#include "LaserShooter.h"
#include "BallSystem.h"

struct PowerUp {
    enum class PowerUpType {
        None,
        MultiBall,
        FastBall,
        SlowBall,
        ExpandPaddle,
        ShrinkPaddle,
        Laser,
        StrongBall,
        FireBall,
        ExtraLife,
        Score50,
        Score100,
        Score250,
        Score500
    };

    PowerUp(float x, float y, PowerUpType type)
            : bounds{x, y, 120, 40}, powerUpType{type} {}

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

        if (levelSystem) {
            levelSystem->onDestroyBrick.subscribe([this](const Brick& brick) {
                int rnd = std::rand() % 100;
                if(rnd < 50) {
                    createPowerUp(brick.getBounds().x + 10, brick.getBounds().y + 5, getRandomPowerUpType());
                }
            });

            levelSystem->onlevelStart.subscribe([this]() {
                powerUps.clear();
            });
        }
    }

    void onDestroy() override {
    }

    void onUpdate() override {
        for (auto& powerup : powerUps) {
            powerup.bounds.y -= 5;

            if(paddle->getBounds().overlaps(powerup.bounds))
            {
                ApplyEffect(powerup.powerUpType);
                onPickup.invoke();
                removePowerUp(powerup);
            }
        }
    }

    void createPowerUp(float x, float y, PowerUp::PowerUpType powerUpType) {
        powerUps.emplace_back(x, y, powerUpType);
    }

    void removePowerUp(const PowerUp& powerUpRef) {
        auto target = std::find_if(powerUps.begin(), powerUps.end(),
            [&](const PowerUp& b) {
                return &b == &powerUpRef;
            });

        if (target != powerUps.end()) {
            powerUps.erase(target);
        }
    }

    void ApplyEffect(PowerUp::PowerUpType powerUpType)
    {
        switch (powerUpType) {
                case PowerUp::PowerUpType::MultiBall:
                    for (const auto &ball: ballSystem->getBalls()) {
                        auto v = ball.velocity;

                        ballSystem->createBall(ball.bounds.x, ball.bounds.y, ball.bounds.w, v.rotate( 0.2f));
                        ballSystem->createBall(ball.bounds.x, ball.bounds.y, ball.bounds.w, v.rotate(-0.2f));
                    }
                    break;

            case PowerUp::PowerUpType::StrongBall:
                ballSystem->setBallType(BallSystem::BallsType::Strong);
                break;

            case PowerUp::PowerUpType::FireBall:
                ballSystem->setBallType(BallSystem::BallsType::Fire);
                break;

            case PowerUp::PowerUpType::Laser:
                laserShooter->activate(true);
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

            default:
                break;
        }
    }

    const std::vector<PowerUp>& getPowerUps() {
        return powerUps;
    }

private:
    std::vector<PowerUp> powerUps;

    std::shared_ptr<LevelSystem> levelSystem;
    std::shared_ptr<BallSystem> ballSystem;
    std::shared_ptr<Paddle> paddle;
    std::shared_ptr<LaserShooter> laserShooter;

    PowerUp::PowerUpType getRandomPowerUpType() {
        static bool seeded = false;
        if (!seeded) {
            std::srand(static_cast<unsigned>(std::time(nullptr)));
            seeded = true;
        }
        int r = std::rand() % 11; // 0..10
        return static_cast<PowerUp::PowerUpType>(r);
    }
};