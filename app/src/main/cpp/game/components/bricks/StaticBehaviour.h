#pragma once

#include "IBrick.h"
#include "IBrickBehaviour.h"

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