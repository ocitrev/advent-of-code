#include "day1.hpp"
#include "../common/assert.hpp"
#include "../common/point2d.hpp"
#include "../common/string.hpp"
#include <fmt/format.h>
#include <unordered_set>

static int Part1(std::string_view directions)
{
    Point2d dir{0, -1};
    Point2d pos{0, 0};

    for (auto d : Split(directions, ", "))
    {
        if (d[0] == 'L')
            dir.Rotate90Left();
        else if (d[0] == 'R')
            dir.Rotate90Right();

        auto const dist = svtoi(d.substr(1));
        pos += dir * dist;
    }

    return pos.Distance();
}

static int Part2(std::string_view directions)
{
    Point2d dir{0, -1};
    Point2d pos{0, 0};
    std::unordered_set<Point2d> visited;

    for (auto d : Split(directions, ", "))
    {
        if (d[0] == 'L')
            dir.Rotate90Left();
        else if (d[0] == 'R')
            dir.Rotate90Right();

        auto const dist = svtoi(d.substr(1));

        for (int i = 0; i != dist; ++i)
        {
            pos += dir;

            if (visited.contains(pos))
                return pos.Distance();

            visited.insert(pos);
        }
    }

    return 0;
}

int main()
{
    // https://adventofcode.com/2016/day/1
    fmt::print("Day 1, 2016: No Time for a Taxicab\n");

    Assert(5 == Part1("R2, L3"));
    Assert(2 == Part1("R2, R2, R2"));
    Assert(12 == Part1("R5, L5, R5, R3"));
    Assert(4 == Part2("R8, R4, R4, R8"));

    auto const part1 = Part1(input::directions);
    fmt::print("  Part 1: {}\n", part1);
    Assert(271 == part1);

    auto const part2 = Part2(input::directions);
    fmt::print("  Part 2: {}\n", part2);
    Assert(153 == part2);
}
