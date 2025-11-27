#pragma once

namespace Breakout {

struct Vector2 {
    float x;
    float y;

    Vector2() : x(0), y(0) {}

    Vector2(float xVal, float yVal) : x(xVal), y(yVal) {}

    Vector2 &operator+=(const Vector2 &other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2 operator+(const Vector2 &other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2 &other) const {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2 rotate(float angleRad) {
        float cosA = std::cos(angleRad);
        float sinA = std::sin(angleRad);
        return {x * cosA - y * sinA,
                x * sinA + y * cosA};
    }

    float length() const {
        return std::sqrt(x * x + y * y);
    }
};

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

    Vector2 center() const {
        return Vector2(x + w * 0.5f, y + h * 0.5f);
    }
};

}