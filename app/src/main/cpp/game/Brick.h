#pragma once

#include "helpers/Math.h"
#include <algorithm>

struct Brick {

public:
    Brick(float x, float y)
            : bounds{x, y, 100, 50} {}

    [[nodiscard]] const Rect& getBounds() const {
        return bounds;
    }
private:
    Rect bounds;
};
