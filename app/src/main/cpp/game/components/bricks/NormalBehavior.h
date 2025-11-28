#pragma once

#include "../brick/IBrick.h"

namespace Breakout {

class NormalBehavior : public IBrick {
public:
    NormalBehavior(int gx, int gy, BrickType type)
            : IBrick(gx, gy, type) {}

    void hit() override {
        isDestroyed = true;
    }
};

}