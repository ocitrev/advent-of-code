// 2019.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <charconv>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include "..\common\input.hpp"

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

int main()
{
    std::vector<int> const list = ReadInts(LR"(C:\Users\eb\OneDrive\devel\python\advent-of-code\2019\1.input)", '\n');

    std::cout << "Part1: " << std::accumulate(begin(list), end(list), 0, [](int r, int mass)
    {
        return r + GetFuel(mass);
    }) << '\n';


    std::cout << "Part2: " << std::accumulate(begin(list), end(list), 0, [](int r, int mass)
    {
        return r + GetRealFuel(mass);
    }) << '\n';
}
