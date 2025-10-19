#include "day3.hpp"
#include "../common.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <gsl/gsl>
#include <map>
#include <string>
#include <vector>

struct Cell
{
    unsigned int flags = 0;
    int length = 0;
};

using Grid = std::map<Point2d, Cell>;

struct Result
{
    int Dist;
    int Length;

    auto operator==(Result const &other) const
    {
        return std::tie(Dist, Length) == std::tie(other.Dist, other.Length);
    }
};

static Point2d GetDirection(char action)
{
    switch (action)
    {
    case 'L':
        return {-1, 0};
    case 'R':
        return {+1, 0};
    case 'U':
        return {0, -1};
    case 'D':
        return {0, +1};
    }

    return {0, 0};
}

static void Run(std::string_view action, unsigned int mask, Point2d &pos, int &length, Grid &grid)
{
    Point2d dir = GetDirection(action.front());
    int dist = svtoi(action.substr(1));

    for (int i = 0; i != dist; ++i)
    {
        pos += dir;
        auto &c = grid[pos];
        c.length += length + i + 1;
        c.flags |= mask;
    }

    length += dist;
}

static Result Solve(std::string_view wire1, std::string_view wire2)
{
    Grid grid;

    {
        int length = 0;
        Point2d pos;
        for (auto const &action : Split(wire1, ','))
        {
            Run(action, 0b01, pos, length, grid);
        }
    }

    {
        int length = 0;
        Point2d pos;
        for (auto const &action : Split(wire2, ','))
        {
            Run(action, 0b10, pos, length, grid);
        }
    }

    std::vector<Result> crossing;

    for (auto const &[p, c] : grid)
    {
        if (c.flags == 0b11)
        {
            crossing.push_back({p.Distance(), c.length});
        }
    }

    Ensures(not crossing.empty());

    auto d = std::min_element(begin(crossing), end(crossing),
        [](auto const &a, auto const &b)
        {
            return a.Dist < b.Dist;
        })->Dist;

    auto l = std::min_element(begin(crossing), end(crossing),
        [](auto const &a, auto const &b)
        {
            return a.Length < b.Length;
        })->Length;

    return {d, l};
}

static auto ParseInput()
{
    auto const lines = Split(GetInput(), '\n');
    return std::make_pair(lines[0], lines[1]);
}

int main()
{
    fmt::print("Day 3, 2019: Crossed Wires\n");
    Assert((Result{6, 30} == Solve("R8,U5,L5,D3", "U7,R6,D4,L4")));
    Assert((Result{159, 610} == Solve("R75,D30,R83,U83,L12,D49,R71,U7,L72", "U62,R66,U55,R34,D71,R55,D58,R83")));
    Assert((Result{135, 410}
            == Solve("R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51", "U98,R91,D20,R16,D67,R40,U7,R15,U6,R7")));

    auto const [wire1, wire2] = ParseInput();
    auto const [closest, fastest] = Solve(wire1, wire2);
    fmt::print("  Part1: {}\n", closest);
    Assert(245 == closest);

    fmt::print("  Part2: {}\n", fastest);
    Assert(48262 == fastest);
}
