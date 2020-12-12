#include "day10.hpp"
#include "../common/assert.hpp"
#include <algorithm>
#include <array>
#include <fmt/format.h>
#include <gsl/gsl>

int Part1(std::vector<int> adapters)
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

int main()
{
    // https://adventofcode.com/2020/day/10
    fmt::print("Day 10: Adapter Array\n");
    Assert(35 == Part1({begin(example::adapters1), end(example::adapters1)}));
    Assert(220 == Part1({begin(example::adapters2), end(example::adapters2)}));

    auto const part1 = Part1({begin(input::adapters), end(input::adapters)});
    fmt::print("  Part 1: {}\n", part1);
    Assert(2059 == part1);
}
