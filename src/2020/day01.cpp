#include "day01.hpp"
#include "../common/assert.hpp"
#include "../common/utils.hpp"
#include <fmt/format.h>
#include <numeric>

static constexpr int kSumWanted = 2020;

static int Find(gsl::span<int const> numbers, std::size_t nb)
{
    int result = 0;
    Combinations(numbers, nb,
        [&](auto const &values)
        {
            if (kSumWanted == std::accumulate(begin(values), end(values), 0, std::plus{}))
            {
                result = std::accumulate(begin(values), end(values), 1, std::multiplies{});
                return false; // break
            }

            return true; // continue
        });

    return result;
}

static void Example()
{
    Assert(514579 == Find(example::numbers, 2));
    Assert(241861950 == Find(example::numbers, 3));
}

static int Part1()
{
    return Find(input::numbers, 2);
}

static int Part2()
{
    return Find(input::numbers, 3);
}

int main()
{
    // https://adventofcode.com/2020/day/1
    fmt::print("Day 1, 2020: Report Repair\n");

    Example();

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(806656 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(230608320 == part2);
}
