#pragma once

#include "../brick/IBrick.h"
#include "../../helpers/GameTime.h"
#include "../../helpers/Event.h"

namespace Breakout {

class ExplodingBehaviour : public IBrick {
public:
    ExplodingBehaviour(int gx, int gy, BrickType type)
            : IBrick(gx, gy, type) {}

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