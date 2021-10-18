#pragma once

#include <functional>

struct Vector2Int
{
    int x;
    int y;

    constexpr Vector2Int() : x{}, y{}
    {
    }

    constexpr Vector2Int(int x, int y) : x(x), y(y)
    {
    }

    friend constexpr Vector2Int operator+(const Vector2Int& left, const Vector2Int& right)
    {
        return Vector2Int(left.x + right.x, left.y + right.y);
    }
    friend constexpr Vector2Int operator-(const Vector2Int& left, const Vector2Int& right)
    {
        return Vector2Int(left.x - right.x, left.y - right.y);
    }
    friend constexpr Vector2Int operator*(const Vector2Int& v, int k)
    {
        return Vector2Int(v.x * k, v.y * k);
    }

    constexpr Vector2Int& operator+=(const Vector2Int& right)
    {
        x += right.x;
        y += right.y;

        return *this;
    }
    constexpr Vector2Int& operator-=(const Vector2Int& right)
    {
        x -= right.x;
        y -= right.y;

        return *this;
    }

    friend constexpr Vector2Int operator-(const Vector2Int& v)
    {
        return Vector2Int(-v.x, -v.y);
    }

    friend constexpr bool operator==(const Vector2Int& left, const Vector2Int& right)
    {
        return left.x == right.x && left.y == right.y;
    }

    static constexpr Vector2Int Left()
    {
        return Vector2Int(-1, 0);
    }
    static constexpr Vector2Int Right()
    {
        return Vector2Int(1, 0);
    }
    static constexpr Vector2Int Up()
    {
        return Vector2Int(0, 1);
    }
    static constexpr Vector2Int Down()
    {
        return Vector2Int(0, -1);
    }
};

struct Vector2IntHasher
{
    std::size_t operator()(const Vector2Int& v) const noexcept
    {
        std::size_t h1 = std::hash<int>{}(v.x);
        std::size_t h2 = std::hash<int>{}(v.y);
        return h1 ^ (h2 << 1);
    }
};
