#pragma once
#include <cmath>
#include <numeric>
#include <ostream>
#include <tuple>

struct Point2d
{
    int x = 0;
    int y = 0;

    friend bool operator<(Point2d const &a, Point2d const &b)
    {
        return std::tie(a.x, a.y) < std::tie(b.x, b.y);
    }

    friend bool operator>(Point2d const &a, Point2d const &b)
    {
        return std::tie(a.x, a.y) > std::tie(b.x, b.y);
    }

    friend bool operator==(Point2d const &a, Point2d const &b)
    {
        return std::tie(a.x, a.y) == std::tie(b.x, b.y);
    }

    friend bool operator!=(Point2d const &a, Point2d const &b)
    {
        return std::tie(a.x, a.y) != std::tie(b.x, b.y);
    }

    [[nodiscard]] Point2d operator+(Point2d const other) const
    {
        Point2d p = *this;
        p += other;
        return p;
    }

    Point2d &operator+=(Point2d const other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    [[nodiscard]] Point2d operator-(Point2d const other) const
    {
        Point2d p = *this;
        p -= other;
        return p;
    }

    Point2d &operator-=(Point2d const other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Point2d &operator*=(int magnitude)
    {
        x *= magnitude;
        y *= magnitude;
        return *this;
    }

    [[nodiscard]] Point2d operator*(int magnitude) const
    {
        Point2d p = *this;
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

    friend std::ostream &operator<<(std::ostream &out, Point2d const &p)
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
    struct hash<::Point2d>
    {
        template <class T>
        static inline void hash_combine(std::size_t &seed, T const &v)
        {
            seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }

        size_t operator()(::Point2d const &p) const noexcept
        {
            size_t hash = 0;
            hash_combine(hash, p.x);
            hash_combine(hash, p.y);
            return hash;
        }
    };
}
