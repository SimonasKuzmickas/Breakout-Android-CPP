#pragma once
#include "Vector2.h"

namespace Breakout {

struct Rect {
    float x, y, w, h;

    Rect() : x(0), y(0), w(0), h(0) {}
    Rect(float xVal, float yVal, float wVal, float hVal)
            : x(xVal), y(yVal), w(wVal), h(hVal) {}

    bool overlaps(const Rect &other) const {
        return !(x + w <= other.x ||
                 other.x + other.w <= x ||
                 y + h <= other.y ||
                 other.y + other.h <= y);
    }

    float left() const { return y; }
    float right() const { return x + w; }
    float top() const { return y + h; }
    float bottom() const { return y; }
    Vector2 center() const { return Vector2(x + w * 0.5f, y + h * 0.5f); }
};

}