#pragma once
#include <cmath>
#include <numeric>
#include <ostream>
#include <tuple>

struct Point
{
    int x = 0;
    int y = 0;

    friend bool operator<(Point const &a, Point const &b)
    {
        return std::tie(a.x, a.y) < std::tie(b.x, b.y);
    }

    friend bool operator>(Point const &a, Point const &b)
    {
        return std::tie(a.x, a.y) > std::tie(b.x, b.y);
    }

    friend bool operator==(Point const &a, Point const &b)
    {
        return std::tie(a.x, a.y) == std::tie(b.x, b.y);
    }

    friend bool operator!=(Point const &a, Point const &b)
    {
        return std::tie(a.x, a.y) != std::tie(b.x, b.y);
    }

    Point operator+(Point const other) const
    {
        Point p = *this;
        return p += other;
    }

    Point &operator+=(Point const other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Point operator-(Point const other) const
    {
        Point p = *this;
        return p -= other;
    }

    Point &operator-=(Point const other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    double Normalize()
    {
        double vecLength = std::hypot(x, y);
        int gcd = std::gcd(x, y);
        x /= gcd;
        y /= gcd;
        return vecLength;
    }

    [[nodiscard]] double Slope() const
    {
        return std::atan2(y, x);
    }

    friend std::ostream &operator<<(std::ostream &out, Point const &p)
    {
        return out << '(' << p.x << ", " << p.y << ')';
    }

    [[nodiscard]] int Distance() const
    {
        return std::abs(x) + std::abs(y);
    }
};

namespace std
{
    template <>
    struct hash<::Point>
    {
        size_t operator()(::Point const &p) const noexcept
        {
            auto h1 = hash<int>{}(p.x);
            auto h2 = hash<int>{}(p.y);
            return h1 ^ (h2 << 1);
        }
    };
}