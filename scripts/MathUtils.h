#pragma once

#include <cmath>

struct vector2
{
    float x;
    float y;
    vector2 normalized()
    {
        float magnitude = std::sqrt(x * x + y * y);
        float normalizedX = 0;
        float normalizedY = 0;
        if (magnitude > 0)
        {
            normalizedX = x / magnitude;
            normalizedY = y / magnitude;
        }
        return { normalizedX, normalizedY };
    }
    static const vector2 right;
    static const vector2 left;
    static const vector2 down;
    static const vector2 up;
    static const vector2 zero;
};

inline const vector2 vector2::right = { 1, 0 };
inline const vector2 vector2::left = { -1, 0 };
inline const vector2 vector2::down = { 0, -1 };
inline const vector2 vector2::up = { 0, 1 };
inline const vector2 vector2::zero = { 0, 0 };