#pragma once

#include "Brick.h"

namespace Breakout {

class StaticBrick : public Brick {
public:
    StaticBrick(int gx, int gy, int look)
            : Brick(gx, gy, look) {
        isDestructible = false;
    }

    void hit() override {
        onDeflect.invoke();
    }
};

}