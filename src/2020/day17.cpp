#include "day17.hpp"
#include "../common/assert.hpp"
#include "../common/point3d.hpp"
#include "../common/point4d.hpp"
#include "../common/string.hpp"
#include <fmt/format.h>
#include <unordered_map>

template <typename T>
using Map = std::unordered_map<T, bool>;

struct GOL3d
{
    Map<Point3d> map;

    void Parse(std::string_view mapText)
    {
        Point3d pos{};

        for (auto &&line : Split(mapText, "\n"))
        {
            pos.x = 0;

            for (char c : line)
            {
                map[pos] = c == '#';
                ++pos.x;
            }

            ++pos.y;
        }
    }

    [[nodiscard]] int CountAliveNeighbors(Point3d pos) const
    {
        int nbNeighbors = 0;

        for (int x = -1; x < 2; ++x)
            for (int y = -1; y < 2; ++y)
                for (int z = -1; z < 2; ++z)
                {
                    Point3d const offset{x, y, z};

                    // skip le centre
                    if (offset == Point3d{})
                        continue;

                    if (IsAlive(pos + offset))
                        ++nbNeighbors;
                }

        return nbNeighbors;
    }

    [[nodiscard]] bool IsAlive(Point3d p) const
    {
        if (auto iter = map.find(p); iter != end(map))
            return iter->second;

        return false;
    }

    void StepOnce()
    {
        Map<Point3d> newMap;
        std::vector<Point3d> points(map.size());

        std::transform(begin(map), end(map), begin(points),
            [](auto const &pair)
            {
                return pair.first;
            });

        const auto [min, max] = std::minmax_element(begin(points), end(points));

        for (int x = min->x - 1; x <= max->x + 1; ++x)
            for (int y = min->y - 1; y <= max->y + 1; ++y)
                for (int z = min->z - 1; z <= max->z + 1; ++z)
                {
                    Point3d const pos{x, y, z};
                    auto const nbNeighbors = CountAliveNeighbors(pos);

                    if (IsAlive(pos))
                        newMap[pos] = nbNeighbors == 2 || nbNeighbors == 3;
                    else
                        newMap[pos] = nbNeighbors == 3;
                }

        map = newMap;
    }

    int BootSequence()
    {
        StepOnce();
        StepOnce();
        StepOnce();
        StepOnce();
        StepOnce();
        StepOnce();
        return CountAlive();
    }

    [[nodiscard]] int CountAlive() const
    {
        return static_cast<int>(std::count_if(begin(map), end(map),
            [](auto const &elem)
            {
                return elem.second;
            }));
    }
};

struct GOL4d
{
    Map<Point4d> map;

    void Parse(std::string_view mapText)
    {
        Point4d pos{};

        for (auto &&line : Split(mapText, "\n"))
        {
            pos.x = 0;

            for (char c : line)
            {
                map[pos] = c == '#';
                ++pos.x;
            }

            ++pos.y;
        }
    }

    [[nodiscard]] int CountAliveNeighbors(Point4d pos) const
    {
        int nbNeighbors = 0;

        for (int x = -1; x < 2; ++x)
            for (int y = -1; y < 2; ++y)
                for (int z = -1; z < 2; ++z)
                    for (int w = -1; w < 2; ++w)
                    {
                        Point4d const offset{x, y, z, w};

                        // skip le centre
                        if (offset == Point4d{})
                            continue;

                        if (IsAlive(pos + offset))
                            ++nbNeighbors;
                    }

        return nbNeighbors;
    }

    [[nodiscard]] bool IsAlive(Point4d p) const
    {
        if (auto iter = map.find(p); iter != end(map))
            return iter->second;

        return false;
    }

    void StepOnce()
    {
        Map<Point4d> newMap;
        std::vector<Point4d> points(map.size());

        std::transform(begin(map), end(map), begin(points),
            [](auto const &pair)
            {
                return pair.first;
            });

        const auto [min, max] = std::minmax_element(begin(points), end(points));

        for (int x = min->x - 1; x <= max->x + 1; ++x)
            for (int y = min->y - 1; y <= max->y + 1; ++y)
                for (int z = min->z - 1; z <= max->z + 1; ++z)
                    for (int w = min->w - 1; w <= max->w + 1; ++w)
                    {
                        Point4d const pos{x, y, z, w};
                        auto const nbNeighbors = CountAliveNeighbors(pos);

                        if (IsAlive(pos))
                            newMap[pos] = nbNeighbors == 2 || nbNeighbors == 3;
                        else
                            newMap[pos] = nbNeighbors == 3;
                    }

        map = newMap;
    }

    int BootSequence()
    {
        StepOnce();
        StepOnce();
        StepOnce();
        StepOnce();
        StepOnce();
        StepOnce();
        return CountAlive();
    }

    [[nodiscard]] int CountAlive() const
    {
        return static_cast<int>(std::count_if(begin(map), end(map),
            [](auto const &elem)
            {
                return elem.second;
            }));
    }
};

static void Example()
{
#ifndef NDEBUG
    GOL3d conwaysCube;
    conwaysCube.Parse(example::start);
    Assert(112 == conwaysCube.BootSequence());

    GOL4d conwaysCube4;
    conwaysCube4.Parse(example::start);
    Assert(848 == conwaysCube4.BootSequence());
#endif
}

static int Part1()
{
    GOL3d conwaysCube;
    conwaysCube.Parse(GetInput());
    return conwaysCube.BootSequence();
}

static int Part2()
{
    GOL4d conwaysCube;
    conwaysCube.Parse(GetInput());
    return conwaysCube.BootSequence();
}

int main()
{
    // https://adventofcode.com/2020/day/17
    fmt::print("Day 17, 2020: Conway Cubes\n");

    Example();

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(255 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(2340 == part2);
}
