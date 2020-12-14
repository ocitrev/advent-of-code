#include "day13.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include <fmt/format.h>
#include <vector>

int Part1()
{
    int min = 0;
    int bestId = 0;

    for (auto &&idText : Split(input::busIds, ','))
    {
        if (idText == "x")
            continue;

        int const id = svtoi(idText);
        
        if (id == 0)
            continue;

        int const start = input::earliest - (input::earliest % id) + id;

        if (min == 0)
        {
            min = start;
            bestId = id;
        }
        else if (start < min)
        {
            min = start;
            bestId = id;
        }
    }

    return bestId * (min - input::earliest);
}

int64_t GetNextStart(std::string_view busIds)
{
    // fortement inspiré de cette solution: https://github.com/colinodell/advent-2020/blob/main/day13/day13.go
    std::vector<int> offsets;

    for (auto &&idText : Split(busIds, ','))
    {
        if (idText == "x")
            offsets.emplace_back(1);
        else
            offsets.emplace_back(svtoi(idText));
    }

    int64_t start = 1;

    for (;;)
    {
        int64_t timeToSkip = 1;
        bool valid = true;
        auto const len = static_cast<int64_t>(offsets.size());

        for (int64_t i = 0; i < len; ++i)
        {
            auto const b = offsets[static_cast<size_t>(i)];
            if ((start + i) % b != 0)
            {
                valid = false;
                break;
            }
            timeToSkip *= b;
        }

        if (valid)
            return start;

        start += timeToSkip;
    }
}

int64_t Part2()
{
    return GetNextStart(input::busIds);
}

int main()
{
    // https://adventofcode.com/2020/day/13
    fmt::print("Day 13: Shuttle Search\n");

    Assert(3417 == GetNextStart("17,x,13,19"));
    Assert(754018 == GetNextStart("67,7,59,61"));
    Assert(779210 == GetNextStart("67,x,7,59,61"));
    Assert(1261476 == GetNextStart("67,7,x,59,61"));
    Assert(1202161486 == GetNextStart("1789,37,47,1889"));

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(261 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(807435693182510 == part2);
}
