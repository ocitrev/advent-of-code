#pragma once
#include <cmath>
#include <numeric>
#include <ostream>
#include <tuple>

struct Point4d
{
    int x = 0;
    int y = 0;
    int z = 0;
    int w = 0;

    friend bool operator<(Point4d const &a, Point4d const &b)
    {
        return std::tie(a.x, a.y, a.z, a.w) < std::tie(b.x, b.y, b.z, b.w);
    }

    friend bool operator>(Point4d const &a, Point4d const &b)
    {
        return std::tie(a.x, a.y, a.z, a.w) > std::tie(b.x, b.y, b.z, b.w);
    }

    friend bool operator==(Point4d const &a, Point4d const &b)
    {
        return std::tie(a.x, a.y, a.z, a.w) == std::tie(b.x, b.y, b.z, b.w);
    }

    friend bool operator!=(Point4d const &a, Point4d const &b)
    {
        return std::tie(a.x, a.y, a.z, a.w) != std::tie(b.x, b.y, b.z, b.w);
    }

    [[nodiscard]] Point4d operator+(Point4d const other) const
    {
        Point4d p = *this;
        p += other;
        return p;
    }

    Point4d &operator+=(Point4d const other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    [[nodiscard]] Point4d operator-(Point4d const other) const
    {
        Point4d p = *this;
        p -= other;
        return p;
    }

    Point4d &operator-=(Point4d const other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }

    Point4d &operator*=(int magnitude)
    {
        x *= magnitude;
        y *= magnitude;
        z *= magnitude;
        w *= magnitude;
        return *this;
    }

    [[nodiscard]] Point4d operator*(int magnitude) const
    {
        Point4d p = *this;
        p *= magnitude;
        return p;
    }

    double Normalize()
    {
        double const xx = x;
        double const yy = y;
        double const zz = z;
        double const ww = w;
        double vecLength = sqrt(xx * xx + yy * yy + zz * zz + ww * ww);
        x = static_cast<int>(round(xx / vecLength));
        y = static_cast<int>(round(yy / vecLength));
        z = static_cast<int>(round(zz / vecLength));
        w = static_cast<int>(round(ww / vecLength));
        return vecLength;
    }

    friend std::ostream &operator<<(std::ostream &out, Point4d const &p)
    {
        return out << '(' << p.x << ", " << p.y << ", " << p.z << ", " << p.w << ')';
    }
};

template <>
struct std::hash<Point4d>
{
    template <class T>
    static inline void hash_combine(std::size_t &seed, T const &v)
    {
        seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    std::size_t operator()(Point4d const &p) const noexcept
    {
        std::size_t value = 0;
        hash_combine(value, p.x);
        hash_combine(value, p.y);
        hash_combine(value, p.z);
        hash_combine(value, p.w);
        return value;
    }
};
