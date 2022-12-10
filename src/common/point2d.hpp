#pragma once
#include <cmath>
#include <numeric>
#include <ostream>
#include <tuple>

struct Point2d
{
    int x = 0;
    int y = 0;

    constexpr Point2d() noexcept = default;

    constexpr Point2d(int x_, int y_) noexcept
        : x(x_)
        , y(y_)
    {
    }

    static Point2d const NORTH;
    static Point2d const SOUTH;
    static Point2d const EAST;
    static Point2d const WEST;
    static Point2d const NORTH_EAST;
    static Point2d const NORTH_WEST;
    static Point2d const SOUTH_EAST;
    static Point2d const SOUTH_WEST;

    constexpr friend bool operator<(Point2d const &a, Point2d const &b)
    {
        return std::tie(a.x, a.y) < std::tie(b.x, b.y);
    }

    constexpr friend bool operator<=(Point2d const &a, Point2d const &b)
    {
        return std::tie(a.x, a.y) <= std::tie(b.x, b.y);
    }

    constexpr friend bool operator>(Point2d const &a, Point2d const &b)
    {
        return std::tie(a.x, a.y) > std::tie(b.x, b.y);
    }

    constexpr friend bool operator>=(Point2d const &a, Point2d const &b)
    {
        return std::tie(a.x, a.y) >= std::tie(b.x, b.y);
    }

    constexpr friend bool operator==(Point2d const &a, Point2d const &b)
    {
        return std::tie(a.x, a.y) == std::tie(b.x, b.y);
    }

    constexpr friend bool operator!=(Point2d const &a, Point2d const &b)
    {
        return std::tie(a.x, a.y) != std::tie(b.x, b.y);
    }

    [[nodiscard]] constexpr Point2d operator+(Point2d const &other) const
    {
        Point2d p = *this;
        p += other;
        return p;
    }

    constexpr Point2d &operator+=(Point2d const &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    [[nodiscard]] constexpr Point2d operator-(Point2d const &other) const
    {
        Point2d p = *this;
        p -= other;
        return p;
    }

    constexpr Point2d &operator-=(Point2d const &other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    constexpr Point2d &operator*=(int magnitude)
    {
        x *= magnitude;
        y *= magnitude;
        return *this;
    }

    constexpr Point2d &operator/=(int magnitute)
    {
        x /= magnitute;
        y /= magnitute;
        return *this;
    }

    [[nodiscard]] constexpr Point2d operator*(int magnitude) const
    {
        Point2d p = *this;
        p *= magnitude;
        return p;
    }

    [[nodiscard]] constexpr Point2d operator*(Point2d const &right) const
    {
        Point2d left = *this;
        left.x *= right.x;
        left.y *= right.y;
        return left;
    }

    [[nodiscard]] constexpr Point2d operator/(int magnitude) const
    {
        Point2d p = *this;
        p /= magnitude;
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

    [[nodiscard]] constexpr double Slope() const
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

    constexpr void Rotate90Left()
    {
        y = -std::exchange(x, y);
    }

    constexpr void Rotate90Right()
    {
        y = std::exchange(x, -y);
    }

    constexpr void Rotate180()
    {
        x = -x;
        y = -y;
    }

    constexpr bool IsVertical() const
    {
        if (x == 0)
            return true;

        return fabs(static_cast<double>(y) / static_cast<double>(x)) > 1.0;
    }

    constexpr bool IsHorizontal() const
    {
        if (y == 0)
            return true;

        return fabs(static_cast<double>(y) / static_cast<double>(x)) < 1.0;
    }
};

inline constexpr Point2d const Point2d::NORTH{0, -1};
inline constexpr Point2d const Point2d::SOUTH{0, 1};
inline constexpr Point2d const Point2d::EAST{1, 0};
inline constexpr Point2d const Point2d::WEST{-1, 0};
inline constexpr Point2d const Point2d::NORTH_EAST = Point2d::NORTH + Point2d::EAST;
inline constexpr Point2d const Point2d::NORTH_WEST = Point2d::NORTH + Point2d::WEST;
inline constexpr Point2d const Point2d::SOUTH_EAST = Point2d::SOUTH + Point2d::EAST;
inline constexpr Point2d const Point2d::SOUTH_WEST = Point2d::SOUTH + Point2d::WEST;

[[nodiscard]] inline double Distance(Point2d a, Point2d b) noexcept
{
    auto const d = a - b;
    return std::hypot(d.x, d.y);
}

inline bool equals(double a, double b, double toler = 1.0e-6) noexcept
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
