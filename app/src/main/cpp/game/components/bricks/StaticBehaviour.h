#pragma once

#include "../brick/IBrick.h"

namespace Breakout {

class StaticBehaviour : public IBrick {
public:
    StaticBehaviour(int gx, int gy, BrickType type)
            : IBrick(gx, gy, type) {}

    void hit() override {
        onDeflect.invoke();
    }
};

}