#pragma once
#include <cmath>
#include <numeric>
#include <ostream>
#include <tuple>

struct Point3d
{
    int x = 0;
    int y = 0;
    int z = 0;

    friend bool operator<(Point3d const &a, Point3d const &b)
    {
        return std::tie(a.x, a.y, a.z) < std::tie(b.x, b.y, b.z);
    }

    friend bool operator>(Point3d const &a, Point3d const &b)
    {
        return std::tie(a.x, a.y, a.z) > std::tie(b.x, b.y, b.z);
    }

    friend bool operator==(Point3d const &a, Point3d const &b)
    {
        return std::tie(a.x, a.y, a.z) == std::tie(b.x, b.y, b.z);
    }

    friend bool operator!=(Point3d const &a, Point3d const &b)
    {
        return std::tie(a.x, a.y, a.z) != std::tie(b.x, b.y, b.z);
    }

    [[nodiscard]] Point3d operator+(Point3d const other) const
    {
        Point3d p = *this;
        p += other;
        return p;
    }

    Point3d &operator+=(Point3d const other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    [[nodiscard]] Point3d operator-(Point3d const other) const
    {
        Point3d p = *this;
        p -= other;
        return p;
    }

    Point3d &operator-=(Point3d const other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Point3d &operator*=(int magnitude)
    {
        x *= magnitude;
        y *= magnitude;
        z *= magnitude;
        return *this;
    }

    [[nodiscard]] Point3d operator*(int magnitude) const
    {
        Point3d p = *this;
        p *= magnitude;
        return p;
    }

    double Normalize()
    {
        double const xx = x;
        double const yy = y;
        double const zz = z;
        double vecLength = sqrt(xx * xx + yy * yy + zz * zz);
        x = static_cast<int>(round(xx / vecLength));
        y = static_cast<int>(round(yy / vecLength));
        z = static_cast<int>(round(zz / vecLength));
        return vecLength;
    }

    friend std::ostream &operator<<(std::ostream &out, Point3d const &p)
    {
        return out << '(' << p.x << ", " << p.y << ", " << p.z << ')';
    }
};

template <>
struct std::hash<Point3d>
{
    template <class T>
    static inline void hash_combine(std::size_t &seed, T const &v)
    {
        seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    std::size_t operator()(Point3d const &p) const noexcept
    {
        std::size_t value = 0;
        hash_combine(value, p.x);
        hash_combine(value, p.y);
        hash_combine(value, p.z);
        return value;
    }
};
