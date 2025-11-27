#pragma once

namespace Breakout {

struct Laser {
    Rect bounds;
    static constexpr float WIDTH = 10.0f;
    static constexpr float HEIGHT = 40.0f;

    Laser(float x, float y) : bounds{x, y, WIDTH, HEIGHT} {}
};

class LaserShooter : public ISceneComponent {
public:
    Event<> onLaserShoot;

    LaserShooter(std::shared_ptr<Paddle> paddle, std::shared_ptr<LevelSystem>  levelSystem, std::shared_ptr<BallSystem> ballSystem)
                : paddle(paddle), levelSystem(levelSystem), ballSystem(ballSystem)
        {}

    void onAwake() override {
        ballSystem->onLost.addListener([this]() {
            setActive(false);
            lasers.clear();
        });

        levelSystem->onLevelStart.addListener([this]() {
            setActive(false);
            lasers.clear();
        });
    }

    void onUpdate() override {
        for (auto &laser: lasers) {
            laser.bounds.y += LASER_SPEED * GameTime::deltaTime();

            if (Brick *brick = levelSystem->checkBrickCollision(laser.bounds)) {
                brick->hit();
                removeLaser(laser);
            } else if (!laser.bounds.overlaps(levelSystem->getLevelBounds())) {
                removeLaser(laser);
            }
        }

        if (getIsActive()) {
            shootTime -= GameTime::deltaTime();
            if (shootTime < 0) {
                onLaserShoot.invoke();
                shootTime = SHOOT_COOLDOWN;

                auto pb = paddle->getBounds();
                createLaser(pb.x, pb.y + pb.h);
                createLaser(pb.x + pb.w, pb.y + pb.h);
            }
        }
    }

    std::vector<Laser> &getLasers() { return lasers; }
    void setActive(bool active) { isActive = active; }
    bool getIsActive() { return isActive; }

private:
    static constexpr float LASER_SPEED = 800.0f;
    static constexpr float SHOOT_COOLDOWN = 1.0f;

    std::shared_ptr<Paddle> paddle;
    std::shared_ptr<LevelSystem> levelSystem;
    std::shared_ptr<BallSystem> ballSystem;

    std::vector<Laser> lasers;
    float shootTime = SHOOT_COOLDOWN;
    bool isActive = false;

    void createLaser(float x, float y) { lasers.emplace_back(x, y); }

    void removeLaser(const Laser &laserRef) {
        auto target = std::find_if(lasers.begin(), lasers.end(),
           [&](const Laser &b) {
               return &b == &laserRef;
           });

        if (target != lasers.end()) {
            lasers.erase(target);
        }
    }
};

}