
#include <cmath>

#pragma once
class Vector2D {
public:
    double x, y;

    // Constructors
    Vector2D() : x(0.0), y(0.0) {}
    Vector2D(double x, double y) : x(x), y(y) {}

    // Vector operations
    double magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    void normalize() {
        double mag = magnitude();
        if (mag != 0.0) {
            x /= mag;
            y /= mag;
        }
    }
};

