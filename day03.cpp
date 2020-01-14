#include "day03.hpp"
#include "common/main.hpp"
#include <algorithm>
#include <cassert>
#include <charconv>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct Cell
{
    unsigned int flags = 0;
    int length = 0;
};

struct Point
{
    int x = 0;
    int y = 0;

    friend bool operator<(Point const &a, Point const &b)
    {
        return std::tie(a.x, a.y) < std::tie(b.x, b.y);
    }

    friend bool operator==(Point const &a, Point const &b)
    {
        return std::tie(a.x, a.y) == std::tie(b.x, b.y);
    }

    Point &operator+=(Point const other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &out, Point const &p)
    {
        return out << '(' << p.x << ", " << p.y << ')';
    }

    int Distance() const
    {
        return abs(x) + abs(y);
    }
};

using Grid = std::map<Point, Cell>;

static std::vector<std::string> Split(std::string_view text, char const sep)
{
    std::vector<std::string> result;

    auto first = begin(text);
    auto iter = first;
    auto const last = end(text);

    for (; iter != last; ++iter)
    {
        if (*iter != sep)
            continue;

        result.emplace_back(first, iter);
        first = iter + 1;
    }

    result.emplace_back(first, iter);

    return result;
}

struct Result
{
    int Dist;
    int Length;

    auto operator==(Result const &other) const
    {
        return std::tie(Dist, Length) == std::tie(other.Dist, other.Length);
    }
};

static Point GetDirection(char action)
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

static void Run(std::string_view action, unsigned int mask, Point &pos, int &length, Grid &grid)
{
    Point dir = GetDirection(action.front());

    int dist;
    [[maybe_unused]] auto result
        = std::from_chars(action.data() + 1, action.data() + action.size(), dist);
    assert(result.ec == std::errc{});

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
        Point pos;
        for (auto const &action : Split(wire1, ','))
        {
            Run(action, 0b01, pos, length, grid);
        }
    }

    {
        int length = 0;
        Point pos;
        for (auto const &action : Split(wire2, ','))
        {
            Run(action, 0b10, pos, length, grid);
        }
    }

    std::vector<Result> crossing;

    for (auto const [p, c] : grid)
    {
        if (c.flags == 0b11)
        {
            crossing.push_back({p.Distance(), c.length});
        }
    }

    auto d = std::min_element(begin(crossing), end(crossing), [](auto const &a, auto const &b) {
                 return a.Dist < b.Dist;
             })->Dist;

    auto l = std::min_element(begin(crossing), end(crossing), [](auto const &a, auto const &b) {
                 return a.Length < b.Length;
             })->Length;

    return {d, l};
}

void Main()
{
    std::cout << "Day 3: Crossed Wires\n";
    assert((Result{6, 30} == Solve("R8,U5,L5,D3", "U7,R6,D4,L4")));
    assert((Result{159, 610}
            == Solve("R75,D30,R83,U83,L12,D49,R71,U7,L72", "U62,R66,U55,R34,D71,R55,D58,R83")));
    assert((Result{135, 410}
            == Solve("R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51",
                     "U98,R91,D20,R16,D67,R40,U7,R15,U6,R7")));

    auto [closest, fastest] = Solve(input::wire1, input::wire2);
    std::cout << "Part1: " << closest << '\n';
    std::cout << "Part2: " << fastest << '\n';
}
