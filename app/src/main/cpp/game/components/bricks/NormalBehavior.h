#pragma once

#include "IBrick.h"
#include "IBrickBehaviour.h"

namespace Breakout {

class NormalBehavior : public IBrickBehavior {
public:
    void hit(IBrick& brick) override {
        brick.destroy();
    }
};

}