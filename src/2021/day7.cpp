#include "day7.hpp"
#include "../common/assert.hpp"
#include "../common/numbers.hpp"
#include <fmt/format.h>
#include <numeric>
#include <span>
#include <unordered_map>

static int GetFuelCostSimple(std::span<int const> numbers, int position)
{
    return std::transform_reduce(begin(numbers), end(numbers), 0, std::plus{},
                                 [position](int n)
                                 {
                                     return abs(n - position);
                                 });
}

static int GetFuelCostTriangular(std::span<int const> numbers, int position)
{
    return std::transform_reduce(begin(numbers), end(numbers), 0, std::plus{},
                                 [position](int n)
                                 {
                                     return Triangular(abs(n - position));
                                 });
}

template <typename FuelCostFunc>
static int Compute(std::span<int const> numbers, FuelCostFunc &&fuelCost)
{
    auto const [min, max] = std::ranges::minmax(numbers);
    int fuel = fuelCost(numbers, min);

    for (int i = min + 1; i != max; ++i)
    {
        auto const f = fuelCost(numbers, i);

        if (f < fuel)
            fuel = f;
    }

    return fuel;
}

static int Part1(std::span<int const> numbers)
{
    return Compute(numbers, &GetFuelCostSimple);
}

static int Part2(std::span<int const> numbers)
{
    return Compute(numbers, &GetFuelCostTriangular);
}

int main()
{
    // https://adventofcode.com/2021/day/7
    fmt::print("Day 7, 2021: The Treachery of Whales\n");

    Assert(37 == Part1(example::positions));
    Assert(37 == GetFuelCostSimple(example::positions, 2));
    Assert(206 == GetFuelCostTriangular(example::positions, 2));
    Assert(168 == Part2(example::positions));

    auto const part1 = Part1(input::positions);
    fmt::print("  Part 1: {}\n", part1);
    Assert(323647 == part1);

    auto const part2 = Part2(input::positions);
    fmt::print("  Part 2: {}\n", part2);
    Assert(87640209 == part2);
}
