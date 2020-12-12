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

    [[nodiscard]] Point operator+(Point const other) const
    {
        Point p = *this;
        p += other;
        return p;
    }

    Point &operator+=(Point const other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    [[nodiscard]] Point operator-(Point const other) const
    {
        Point p = *this;
        p -= other;
        return p;
    }

    Point &operator-=(Point const other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Point &operator*=(int magnitude)
    {
        x *= magnitude;
        y *= magnitude;
        return *this;
    }

    [[nodiscard]] Point operator*(int magnitude) const
    {
        Point p = *this;
        p *= magnitude;
        return p;
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

    void Rotate90Left()
    {
        y = -y;
        std::swap(x, y);
    }

    void Rotate90Right()
    {
        x = -x;
        std::swap(x, y);
    }

    void Rotate180()
    {
        x = -x;
        y = -y;
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
