#pragma once

#include "Brick.h"

namespace Breakout {

class ArmorBrick : public Brick {
public:
    ArmorBrick(int gx, int gy, BrickType type)
            : Brick(gx, gy, type) {}

    void hit() override {
        if (!isDamaged) {
            isDamaged = true;
            onDamage.invoke();
        } else {
            isDestroyed = true;
        }
    }
};

}