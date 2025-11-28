#pragma once

#include "Brick.h"

namespace Breakout {

class StaticBrick : public Brick {
public:
    StaticBrick(int gx, int gy, BrickType type)
            : Brick(gx, gy, type) {}

    void hit() override {
        onDeflect.invoke();
    }
};

}