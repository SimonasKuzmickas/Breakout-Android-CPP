#pragma once

struct Ball {
    Rect bounds;
    Vector2 velocity;

    Ball(float x, float y, float radius, const Vector2& v)
            : bounds{x, y, radius, radius}, velocity(v) {}
};