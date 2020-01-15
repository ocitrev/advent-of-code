#include "day01.hpp"
#include "common/main.hpp"
#include <charconv>
#include <fstream>
#include <iostream>
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
    else if (fuel < 0)
        fuel = 0;
    return fuel;
}

static_assert(2 == GetRealFuel(12));
static_assert(2 == GetRealFuel(14));
static_assert(966 == GetRealFuel(1969));
static_assert(50346 == GetRealFuel(100756));

void Main()
{
    std::cout << "Day 1: The Tyranny of the Rocket Equation\n";

    std::cout << "  Part1: "
              << std::accumulate(begin(input::data), end(input::data), 0,
                                 [](int r, int mass) {
                                     return r + GetFuel(mass);
                                 })
              << '\n';

    std::cout << "  Part2: "
              << std::accumulate(begin(input::data), end(input::data), 0,
                                 [](int r, int mass) {
                                     return r + GetRealFuel(mass);
                                 })
              << '\n';
}
