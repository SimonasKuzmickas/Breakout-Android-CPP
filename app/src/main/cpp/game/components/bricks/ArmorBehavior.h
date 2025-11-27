#pragma once

#include "IBrick.h"
#include "IBrickBehaviour.h"

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