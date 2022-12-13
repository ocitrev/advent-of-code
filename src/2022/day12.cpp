#include "day12.hpp"
#include "../common.hpp"
#include "../common/dijkstra.hpp"

struct Map
{
    std::unordered_map<Point2d, int> map;
    Point2d start;
    Point2d target;

    void Parse(std::string_view mapText)
    {
        int y = 0;

        for (auto line : Split(mapText, '\n'))
        {
            int x = 0;

            for (char c : line)
            {
                Point2d p{x++, y};

                if (c == 'S')
                {
                    start = p;
                    c = 'a';
                }
                else if (c == 'E')
                {
                    target = p;
                    c = 'z';
                }

                map[p] = c - 'a';
            }

            ++y;
        }
    }

    static constexpr int kInvalidHeight = -1;

    int GetHeight(Point2d p) const
    {
        if (auto it = map.find(p); it != end(map))
        {
            return it->second;
        }

        return kInvalidHeight;
    }
};

static auto Solve(std::string_view mapText)
{
    Map map;
    map.Parse(mapText);

    Dijkstra solver{map.map,
        [](Point2d from, Point2d to)
        {
            return from.Distance(to);
        },
        [&](Point2d from, Point2d to)
        {
            auto const fromHeight = map.GetHeight(from);
            auto const toHeight = map.GetHeight(to);

            if (fromHeight == Map::kInvalidHeight || toHeight == Map::kInvalidHeight)
                return false;

            int const distance = toHeight - fromHeight;
            return distance <= 1;
        },
        [&](Point2d to)
        {
            return to == map.target;
        }};

    auto const path = solver.Solve(map.start);
    return path.size();
}

static auto SolveReverse(std::string_view mapText)
{
    Map map;
    map.Parse(mapText);

    Dijkstra solver{map.map,
        [](Point2d from, Point2d to)
        {
            return from.Distance(to);
        },
        [&](Point2d from, Point2d to)
        {
            auto const fromHeight = map.GetHeight(from);
            auto const toHeight = map.GetHeight(to);

            if (fromHeight == Map::kInvalidHeight || toHeight == Map::kInvalidHeight)
                return false;

            int const distance = fromHeight - toHeight;
            return distance <= 1;
        },
        [&](Point2d to)
        {
            return map.GetHeight(to) == 0;
        }};

    auto const path = solver.Solve(map.target);
    return path.size();
}

static auto Part1()
{
    return Solve(input::map);
}

static auto Part2()
{
    return SolveReverse(input::map);
}

int main()
{
    // https://adventofcode.com/2022/day/12
    fmt::print("Day 12, 2022: Hill Climbing Algorithm\n");

    Assert(31 == Solve(example::map));
    Assert(29 == SolveReverse(example::map));

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(412 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(402 == part2);
}
