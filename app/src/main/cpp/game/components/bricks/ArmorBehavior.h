#pragma once

#include "../brick/IBrick.h"
#include "../brick/IBrickBehaviour.h"

namespace Breakout {

class ArmorBehavior : public IBrickBehavior {
public:
    void hit(IBrick& brick) override {
        if (!brick.getIsDamaged()) {
            brick.damage();
        } else {
            brick.destroy();
        }
    }
};

}