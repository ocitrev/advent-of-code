#include "day01.hpp"
#include "terminal.hpp"
#include <cassert>
#include <fmt/format.h>
#include <fstream>
#include <numeric>
#include <string>
#include <vector>

constexpr int GetFuel(int mass)
{
    return mass / 3 - 2;
}

static_assert(2 == GetFuel(14));
static_assert(2 == GetFuel(14));
static_assert(654 == GetFuel(1969));
static_assert(33583 == GetFuel(100756));

constexpr int GetRealFuel(int mass)
{
    int fuel = GetFuel(mass);

    if (fuel > 0)
        return fuel + GetRealFuel(fuel);

    if (fuel < 0)
        fuel = 0;

    return fuel;
}

static_assert(2 == GetRealFuel(12));
static_assert(2 == GetRealFuel(14));
static_assert(966 == GetRealFuel(1969));
static_assert(50346 == GetRealFuel(100756));

int main()
{
    fmt::print("Day 1: The Tyranny of the Rocket Equation\n");

    int part1 = std::accumulate(begin(input::data), end(input::data), 0, [](int r, int mass) {
        return r + GetFuel(mass);
    });

    int part2 = std::accumulate(begin(input::data), end(input::data), 0, [](int r, int mass) {
        return r + GetRealFuel(mass);
    });

    fmt::print("  Part1: {}\n", part1);
    assert(3538016 == part1);

    fmt::print("  Part2: {}\n", part2);
    assert(5304147 == part2);
}
