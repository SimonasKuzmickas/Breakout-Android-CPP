#pragma once

#include "../brick/IBrick.h"

namespace Breakout {

class NormalBehavior : public IBrick {
public:
    NormalBehavior(int gx, int gy)
            : IBrick(gx, gy, IBrick::BrickType::NormalOrange) {}

    void hit() override {
        isDestroyed = true;
    }
};

}