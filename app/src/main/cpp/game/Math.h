#pragma once

struct Rect {
    float left, top, right, bottom;

    Rect() : left(0), top(0), right(0), bottom(0) {}  // default
    Rect(float l, float t, float r, float b)
            : left(l), top(t), right(r), bottom(b) {}

    float width() const { return right - left; }
    float height() const { return bottom - top; }
};

struct Vector2 {
    float x;
    float y;

    Vector2() : x(0), y(0) {}

    Vector2(float xVal, float yVal) : x(xVal), y(yVal) {}

    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    float length() const {
        return std::sqrt(x * x + y * y);
    }
};