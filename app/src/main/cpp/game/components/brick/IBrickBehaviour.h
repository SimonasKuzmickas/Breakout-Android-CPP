#pragma once

#include "IBrick.h"

namespace Breakout {

class IBrickBehavior {
public:
    virtual ~IBrickBehavior() = default;
    virtual void start(IBrick& brick) {};
    virtual void update(IBrick& brick) {};
    virtual void hit(IBrick& brick) {};
};

}