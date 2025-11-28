#pragma once

#include "../brick/IBrick.h"
#include "../brick/IBrickBehaviour.h"

namespace Breakout {

class NormalBehavior : public IBrickBehavior {
public:
    void hit(IBrick& brick) override {
        brick.destroy();
    }
};

}