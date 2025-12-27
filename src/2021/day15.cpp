#include "day15.hpp"

#include "../common.hpp"

struct State
{
    int risk;
    Point2d current;
};

struct Map
{
    std::unordered_map<Point2d, int> map;
    int width = 0;
    int height = 0;

    void Parse(std::string_view text)
    {
        int y = 0;

        for (auto line : Split(text, '\n'))
        {
            int x = 0;

            for (char c : line)
            {
                map[{x, y}] = c - '0';
                ++x;
            }

            width = x;
            ++y;
        }

        height = y;
    }

    int Risk(Point2d p) const
    {
        int const offset = p.x / width + p.y / height;
        p.x %= width;
        p.y %= height;
        auto iterTo = map.find(p);

        if (iterTo == end(map))
        {
            throw std::invalid_argument("p");
        }

        return LimitScore(iterTo->second + offset);
    }

    static int LimitScore(int score)
    {
        return ((score - 1) % 9) + 1;
    }

    static auto Neighbors(Point2d current)
    {
        return std::array{current + Point2d::WEST, current + Point2d::NORTH, current + Point2d::EAST, current + Point2d::SOUTH};
    }

    static size_t GetOffset(Point2d p, int w)
    {
        return static_cast<size_t>(p.y * w + p.x);
    }

    static bool Inbound(Point2d const &p, Point2d const &goal)
    {
        return p.x >= 0 && p.y >= 0 && p.x <= goal.x && p.y <= goal.y;
    }

    int Solve(int factor) const
    {
        // based on: https://github.com/willkill07/AdventOfCode2021/blob/main/days/Day15.cpp
        int const w = width * factor;
        int const h = height * factor;
        Point2d const goal{w - 1, h - 1};
        std::vector<std::uint8_t> seen(static_cast<size_t>(w * h));
        std::vector<std::vector<State>> queues(10);

        for (auto &q : queues)
        {
            q.reserve(static_cast<size_t>((w + h) * 2));
        }

        queues.front().push_back({0, Point2d{}});

        while (true)
        {
            for (auto &q : queues)
            {
                while (not q.empty())
                {
                    auto const [risk, current] = q.back();
                    q.pop_back();

                    if (current == goal)
                    {
                        return risk;
                    }

                    if (seen[GetOffset(current, w)])
                    {
                        continue;
                    }

                    seen[GetOffset(current, w)] = 1U;

                    for (auto n : Neighbors(current))
                    {
                        if (not Inbound(n, goal))
                        {
                            continue;
                        }

                        if (not seen[GetOffset(n, w)])
                        {
                            auto const newRisk = risk + Risk(n);
                            queues[static_cast<size_t>(newRisk % 10)].push_back({newRisk, n});
                        }
                    }
                }
            }
        }
    }
};

static int Part1(std::string_view text)
{
    Map m;
    m.Parse(text);
    return m.Solve(1);
}

static int Part2(std::string_view text)
{
    Map m;
    m.Parse(text);
    return m.Solve(5);
}

int main()
{
    // https://adventofcode.com/2021/day/15
    std::println("Day 15, 2021: Chiton");

    Assert(40 == Part1(example::map));
    Assert(315 == Part2(example::map));

    auto const part1 = Part1(GetInput());
    std::println("  Part 1: {}", part1);
    Assert(447 == part1);

    auto const part2 = Part2(GetInput());
    std::println("  Part 2: {}", part2);
    Assert(2825 == part2);
}
