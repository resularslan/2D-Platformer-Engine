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
};