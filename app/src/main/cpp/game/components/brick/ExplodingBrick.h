#pragma once

#include "Brick.h"
#include "../../helpers/GameTime.h"
#include "../../helpers/Event.h"

namespace Breakout {

class ExplodingBrick : public Brick {
public:
    ExplodingBrick(int gx, int gy, BrickType type)
            : Brick(gx, gy, type) {}

    void hit() override {
        if (!isDamaged) {
            isDamaged = true;
            return;
        }
    }

    void update() override {
        if (isDamaged) {
            explosionTimer -= GameTime::deltaTime();
            if(explosionTimer <= 0) {
                onExplode.invoke(this);
                isDestroyed = true;
            }
        }
    }
private:
    float explosionTimer = 0.2f;
};

}