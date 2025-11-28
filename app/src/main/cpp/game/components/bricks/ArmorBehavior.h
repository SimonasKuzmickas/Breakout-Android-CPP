#pragma once

#include "../brick/IBrick.h"

namespace Breakout {

class ArmorBehavior : public IBrick {
public:
    ArmorBehavior(int gx, int gy)
            : IBrick(gx, gy, IBrick::BrickType::ArmorBrown) {}

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