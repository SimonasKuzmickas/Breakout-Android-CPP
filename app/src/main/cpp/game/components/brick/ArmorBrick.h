#pragma once

namespace Breakout {

class ArmorBrick : public Brick {
public:
    ArmorBrick(int gx, int gy, int look)
            : Brick(gx, gy, look) {}

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