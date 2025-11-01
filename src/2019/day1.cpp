#include "day1.hpp"

#include "../common.hpp"

#include <fstream>

constexpr int GetFuel(int mass)
{
    return mass / 3 - 2;
}

static_assert(2 == GetFuel(14));
static_assert(2 == GetFuel(14));
static_assert(654 == GetFuel(1969));
static_assert(33'583 == GetFuel(100'756));

constexpr int GetRealFuel(int mass)
{
    int fuel = GetFuel(mass);

    if (fuel > 0)
    {
        return fuel + GetRealFuel(fuel);
    }

    if (fuel < 0)
    {
        fuel = 0;
    }

    return fuel;
}

static_assert(2 == GetRealFuel(12));
static_assert(2 == GetRealFuel(14));
static_assert(966 == GetRealFuel(1969));
static_assert(50'346 == GetRealFuel(100'756));

int main()
{
    fmt::print("Day 1, 2019: The Tyranny of the Rocket Equation\n");
    auto const input = ParseInputNumbers();

    int const part1 = std::accumulate(begin(input), end(input), 0,
        [](int r, int mass)
        {
            return r + GetFuel(mass);
        });

    int const part2 = std::accumulate(begin(input), end(input), 0,
        [](int r, int mass)
        {
            return r + GetRealFuel(mass);
        });

    fmt::print("  Part1: {}\n", part1);
    Assert(3'538'016 == part1);

    fmt::print("  Part2: {}\n", part2);
    Assert(5'304'147 == part2);
}
