#include "day13.hpp"

#include "../common.hpp"

struct Ticket
{
    int earliest;
    std::string_view busIds;
};

static Ticket ParseInput()
{
    auto parts = Split(GetInput(), '\n');
    return {svtoi(parts[0]), parts[1]};
}

static int Part1()
{
    int min = 0;
    int bestId = 0;
    auto const ticket = ParseInput();

    for (auto &&idText : Split(ticket.busIds, ','))
    {
        if (idText == "x")
        {
            continue;
        }

        int const id = svtoi(idText);

        if (id == 0)
        {
            continue;
        }

        int const start = ticket.earliest - (ticket.earliest % id) + id;

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

    return bestId * (min - ticket.earliest);
}

static int64_t GetNextStart(Ticket const &ticket)
{
    // fortement inspiré de cette solution: https://github.com/colinodell/advent-2020/blob/main/day13/day13.go
    std::vector<int> offsets;

    for (auto &&idText : Split(ticket.busIds, ','))
    {
        if (idText == "x")
        {
            offsets.emplace_back(1);
        }
        else
        {
            offsets.emplace_back(svtoi(idText));
        }
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
        {
            return start;
        }

        start += timeToSkip;
    }
}

static int64_t Part2()
{
    return GetNextStart(ParseInput());
}

int main()
{
    // https://adventofcode.com/2020/day/13
    fmt::print("Day 13, 2020: Shuttle Search\n");

    Assert(3417 == GetNextStart({0, "17,x,13,19"}));
    Assert(754'018 == GetNextStart({0, "67,7,59,61"}));
    Assert(779'210 == GetNextStart({0, "67,x,7,59,61"}));
    Assert(1'261'476 == GetNextStart({0, "67,7,x,59,61"}));
    Assert(1'202'161'486 == GetNextStart({0, "1789,37,47,1889"}));

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(261 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(807'435'693'182'510 == part2);
}
