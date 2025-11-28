#pragma once

#include "../brick/IBrick.h"
#include "../brick/IBrickBehaviour.h"

namespace Breakout {

class StaticBehaviour : public IBrickBehavior {
public:
    void start(IBrick& brick) override {
        brick.setDestructible(false);
    }

    void hit(IBrick& brick) override {
        brick.deflect();
    }
};

}