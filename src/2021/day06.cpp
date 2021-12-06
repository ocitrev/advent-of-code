#include "day06.hpp"
#include "../common/assert.hpp"
#include <algorithm>
#include <array>
#include <fmt/format.h>
#include <numeric>
#include <span>

static uint64_t Run(std::span<int8_t const> fishes, int const days)
{
    std::array<uint64_t, 9> counters{};

    for (int8_t f : fishes)
    {
        ++counters[static_cast<size_t>(f)];
    }

    for (int d = 0; d != days; ++d)
    {
        std::rotate(begin(counters), begin(counters) + 1, end(counters));
        counters[6] += counters[8];
    }

    return std::accumulate(begin(counters), end(counters), uint64_t{0});
}

static uint64_t Part1(std::span<int8_t const> fishes)
{
    return Run(fishes, 80);
}

static uint64_t Part2(std::span<int8_t const> fishes)
{
    return Run(fishes, 256);
}

int main()
{
    // https://adventofcode.com/2021/day/6
    fmt::print("Day 6, 2021: Lanternfish\n");

    Assert(26 == Run(example::fishes, 18));
    Assert(5934 == Run(example::fishes, 80));
    Assert(26984457539 == Run(example::fishes, 256));

    auto const part1 = Part1(input::fishes);
    fmt::print("  Part 1: {}\n", part1);
    Assert(359344 == part1);

    auto const part2 = Part2(input::fishes);
    fmt::print("  Part 2: {}\n", part2);
    Assert(1629570219571 == part2);
}
