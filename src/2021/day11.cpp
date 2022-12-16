#include "day11.hpp"
#include "../common.hpp"

struct Environment
{
    std::unordered_map<Point2d, int8_t> map;
    std::unordered_set<Point2d> flashed;
    int width = 0;
    int height = 0;

    void Parse(std::string_view mapText)
    {
        int y = 0;

        for (auto line : Split(mapText, '\n'))
        {
            int x = 0;

            for (char c : line)
            {
                map[{x, y}] = static_cast<int8_t>(c - '0');
                ++x;
            }

            width = x;
            ++y;
        }

        height = y;
    }

    void Increase(Point2d const &p)
    {
        auto iter = map.find(p);
        if (iter == end(map))
            return;

        auto &level = iter->second;
        ++level;

        if (level > 9)
        {
            Flash(p);
        }
    }

    void Flash(Point2d const &p)
    {
        if (flashed.find(p) != end(flashed))
            return;

        flashed.insert(p);
        Increase(p + Point2d::NORTH);
        Increase(p + Point2d::NORTH_EAST);
        Increase(p + Point2d::EAST);
        Increase(p + Point2d::SOUTH_EAST);
        Increase(p + Point2d::SOUTH);
        Increase(p + Point2d::SOUTH_WEST);
        Increase(p + Point2d::WEST);
        Increase(p + Point2d::NORTH_WEST);
    }

    int Step()
    {
        flashed.clear();

        for (auto &[p, l] : map)
            if (++l > 9)
                Flash(p);

        for (auto &[p, l] : map)
            if (l > 9)
                l = 0;

        return static_cast<int>(flashed.size());
    }

    bool AllFlashed() const
    {
        return flashed.size() == map.size();
    }
};

static int Part1(std::string_view lines, int n)
{
    Environment env;
    env.Parse(lines);
    int flashCount = 0;

    for (int i = 0; i != n; ++i)
        flashCount += env.Step();

    return flashCount;
}

static int Part2(std::string_view lines)
{
    Environment env;
    env.Parse(lines);
    int step = 0;

    while (not env.AllFlashed())
    {
        ++step;
        env.Step();
    }

    return step;
}

int main()
{
    // https://adventofcode.com/2021/day/11
    fmt::print("Day 11, 2021: Dumbo Octopus\n");

    Assert(204 == Part1(example::energyLevels, 10));
    Assert(1656 == Part1(example::energyLevels, 100));
    Assert(195 == Part2(example::energyLevels));

    auto const part1 = Part1(GetInput(), 100);
    fmt::print("  Part 1: {}\n", part1);
    Assert(1793 == part1);

    auto const part2 = Part2(GetInput());
    fmt::print("  Part 2: {}\n", part2);
    Assert(247 == part2);
}
