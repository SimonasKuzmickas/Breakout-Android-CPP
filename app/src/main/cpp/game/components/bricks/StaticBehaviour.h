#pragma once

#include "../brick/IBrick.h"

namespace Breakout {

class StaticBehaviour : public IBrick {
public:
    StaticBehaviour(int gx, int gy)
            : IBrick(gx, gy, IBrick::BrickType::StaticGray) {}

    void hit() override {
        onDeflect.invoke();
    }
};

}