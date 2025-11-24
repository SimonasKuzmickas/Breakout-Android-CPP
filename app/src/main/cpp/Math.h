#pragma once

struct Rect {
    int left, top, right, bottom;

    Rect() : left(0), top(0), right(0), bottom(0) {}  // default
    Rect(int l, int t, int r, int b)
            : left(l), top(t), right(r), bottom(b) {}

    int width() const { return right - left; }
    int height() const { return bottom - top; }
};