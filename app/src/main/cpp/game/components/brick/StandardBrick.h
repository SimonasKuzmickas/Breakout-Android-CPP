#pragma once

#include "Brick.h"

namespace Breakout {

class StandardBrick : public Brick {
public:
    StandardBrick(int gx, int gy, BrickType type)
            : Brick(gx, gy, type) {}

    void hit() override {
        isDestroyed = true;
    }
};

}