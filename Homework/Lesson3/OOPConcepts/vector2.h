#pragma once

struct vector2
{
    constexpr vector2() : x(0.0f), y(0.0f) {}
    constexpr vector2(float x, float y) : x(x), y(y) {}

    constexpr friend vector2 operator+(const vector2& a, const vector2& b)
    {
        return vector2(a.x + b.x, a.y + b.y);
    }
    constexpr friend vector2 operator-(const vector2& a, const vector2& b)
    {
        return vector2(a.x - b.x, a.y - b.y);
    }
    constexpr friend vector2 operator*(const vector2& v, float m)
    {
        return vector2(v.x * m, v.y * m);
    }

    float x;
    float y;
};