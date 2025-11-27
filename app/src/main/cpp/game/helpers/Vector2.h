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

}