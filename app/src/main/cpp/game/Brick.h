#pragma once
#include "Math.h"
#include <algorithm>

struct Brick {

public:
    Brick(float x, float y)
            : bounds{x, y, 100, 50} {}

    [[nodiscard]] const Rect& getBounds() const {   // <-- note the const at the end
        return bounds;
    }
private:
    Rect bounds;
};
