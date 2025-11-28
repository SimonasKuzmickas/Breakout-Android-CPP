#pragma once

#include "../brick/IBrick.h"

namespace Breakout {

class ArmorBehavior : public IBrick {
public:
    ArmorBehavior(int gx, int gy, BrickType type)
            : IBrick(gx, gy, type) {}

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