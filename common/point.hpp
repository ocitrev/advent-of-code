#pragma once
#include <numeric>
#include <ostream>

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
        return atan2(y, x);
    }

    friend std::ostream &operator<<(std::ostream &out, Point const &p)
    {
        return out << '(' << p.x << ", " << p.y << ')';
    }

    [[nodiscard]] int Distance() const
    {
        return abs(x) + abs(y);
    }
};
