#pragma once
#include <cmath>
#include <numeric>
#include <ostream>
#include <tuple>

struct Point2d
{
    int x = 0;
    int y = 0;

    static Point2d const NORTH;
    static Point2d const SOUTH;
    static Point2d const EAST;
    static Point2d const WEST;

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

    [[nodiscard]] Point2d operator*(Point2d const &right) const
    {
        Point2d left = *this;
        left.x *= right.x;
        left.y *= right.y;
        return left;
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

    [[nodiscard]] int Distance() const noexcept
    {
        return std::abs(x) + std::abs(y);
    }

    void Rotate90Left()
    {
        y = -std::exchange(x, y);
    }

    void Rotate90Right()
    {
        y = std::exchange(x, -y);
    }

    void Rotate180()
    {
        x = -x;
        y = -y;
    }
};

inline constexpr Point2d Point2d::NORTH{0, -1};
inline constexpr Point2d Point2d::SOUTH{0, 1};
inline constexpr Point2d Point2d::EAST{1, 0};
inline constexpr Point2d Point2d::WEST{-1, 0};

[[nodiscard]] inline double Distance(Point2d a, Point2d b) noexcept
{
    return sqrt(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)));
}

inline constexpr bool equals(double a, double b, double toler = 1.0e-6) noexcept
{
    return fabs(b - a) < toler;
}

inline bool IsOnLine(Point2d a, Point2d b, Point2d c) noexcept
{
    double const ac = Distance(a, c);
    double const bc = Distance(b, c);
    double const ab = Distance(a, b);
    return equals(ac + bc, ab);
}

template <>
struct std::hash<Point2d>
{
    template <class T>
    static inline void hash_combine(std::size_t &seed, T const &v)
    {
        seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    std::size_t operator()(Point2d const &p) const noexcept
    {
        std::size_t value = 0;
        hash_combine(value, p.x);
        hash_combine(value, p.y);
        return value;
    }
};
