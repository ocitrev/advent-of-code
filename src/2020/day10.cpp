#include "day10.hpp"

#include "../common.hpp"

static int Part1(std::vector<int> adapters)
{
    std::sort(begin(adapters), end(adapters));
    adapters.push_back(adapters.back() + 3);
    std::array<int, 4> diffs{};

    int prev = 0;
    for (int adapter : adapters)
    {
        ++diffs.at(static_cast<size_t>(adapter - prev));
        prev = adapter;
    }

    return diffs[1] * diffs[3];
}

static int Tribbonacci(int n)
{
    if (n < 2)
    {
        return 0;
    }

    int first = 0;
    int second = 0;
    int third = 1;

    for (int i = 0; i < n - 2; ++i)
    {
        int const next = first + second + third;
        first = second;
        second = third;
        third = next;
    }

    return third;
}

static int64_t Part2(std::vector<int> adapters)
{
    // fortement inspiré de cette solution: https://github.com/colinodell/advent-2020/blob/main/day10/day10.go
    adapters.insert(begin(adapters), 0);
    std::sort(begin(adapters), end(adapters));
    adapters.emplace_back(adapters.back() + 3);

    auto const len = static_cast<int>(adapters.size());
    int first = 0;
    int last = 0;
    int64_t result = 1;

    while (first < len - 1 && last < len - 1)
    {
        while (adapters[static_cast<size_t>(last + 1)] - adapters[static_cast<size_t>(last)] == 1)
        {
            ++last;
        }

        int l = last - first + 1;

        if (l > 2)
        {
            result *= Tribbonacci(l + 1);
        }

        first = last + 1;
        last = first;
    }

    return result;
}

int main()
{
    // https://adventofcode.com/2020/day/10
    fmt::print("Day 10, 2020: Adapter Array\n");
    Assert(35 == Part1({begin(example::adapters1), end(example::adapters1)}));
    Assert(220 == Part1({begin(example::adapters2), end(example::adapters2)}));

    auto const part1 = Part1(ParseInputNumbers());
    fmt::print("  Part 1: {}\n", part1);
    Assert(2059 == part1);

    auto const part2 = Part2(ParseInputNumbers());
    fmt::print("  Part 2: {}\n", part2);
    Assert(86'812'553'324'672 == part2);
}
