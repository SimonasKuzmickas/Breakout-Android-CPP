#pragma once

namespace Breakout {

class StandardBrick : public Brick {
public:
    StandardBrick(int gx, int gy, int look)
            : Brick(gx, gy, look) {}

    void hit() override {
        isDestroyed = true;
    }
};

}