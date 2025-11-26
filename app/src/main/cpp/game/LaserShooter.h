#pragma once

struct Laser {
    Rect bounds;

    Laser(float x, float y)
            : bounds{x, y, 10, 40} {}
};

class LaserShooter : public ISceneComponent {
public:
    Event<> onLaserShoot;

    void onAwake() override {
        paddle = getComponent<Paddle>();
        levelSystem = getComponent<LevelSystem>();
    }

    void onDestroy() override {

    }

    void onUpdate() override {

        for (auto& laser : lasers) {
            laser.bounds.y += 10;

            if (Brick* hit = levelSystem->checkBrickCollision(laser.bounds))
            {
                levelSystem->removeBrick(*hit);
                removeLaser(laser);
            }

            if(!laser.bounds.overlaps(levelSystem->getLevelBounds()))
            {
                removeLaser(laser);
            }
        }

        if(getIsActive())
        {
            activeTime -= 0.05f;
            shootTime -= 0.05f;
            if(shootTime < 0)
            {
                onLaserShoot.invoke();

                shootTime = 3;

                auto positionBounds = paddle->getBounds();
                createLaser(positionBounds.x, positionBounds.y + positionBounds.h);
                createLaser(positionBounds.x + positionBounds.w, positionBounds.y + positionBounds.h);
            }
        }
    }

    void createLaser(float x, float y) {
        lasers.emplace_back(x, y);
    }

    const std::vector<Laser>& getLasers() const {
        return lasers;
    }

    void activate(bool active) {
        activeTime = 20;
    }

    bool getIsActive() const {
        return activeTime > 0;
    }

    void removeLaser(const Laser& laserRef) {
        auto target = std::find_if(lasers.begin(), lasers.end(),
           [&](const Laser& b) {
               return &b == &laserRef;
           });

        if (target != lasers.end()) {
            lasers.erase(target);
        }
    }

private:
    std::shared_ptr<Paddle> paddle;
    std::shared_ptr<LevelSystem> levelSystem;

    std::vector<Laser> lasers;

    float shootTime = 0;
    float activeTime = 0;
};
