#include "day04.hpp"
#include "common/main.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

static bool IsIncreasing(int n)
{
    std::string sorted = std::to_string(n);
    std::sort(begin(sorted), end(sorted));
    return n == std::stoi(sorted);
}

constexpr static bool HasDoubleDigit(int n)
{
    std::array<int, 10> digitCount{};

    do
    {
        if (++digitCount[n % 10] > 1)
            return true;

        n /= 10;
    } while (n != 0);

    return false;
}

constexpr static bool HasDoubleDigitExact(int n)
{
    std::array<int, 10> digitCount{};

    do
    {
        ++digitCount[n % 10];
        n /= 10;
    } while (n != 0);

    for (int i : digitCount)
    {
        if (i == 2)
            return true;
    }

    return false;
}

#ifdef DEBUG
static bool IsValid1(int n)
{
    return IsIncreasing(n) && HasDoubleDigit(n);
}

static bool IsValid2(int n)
{
    return IsIncreasing(n) && HasDoubleDigitExact(n);
}
#endif

void Main()
{
    std::cout << "Day 4: Secure Container\n";

    assert(IsIncreasing(111123));
    assert(IsIncreasing(135679));

    assert(IsValid1(111111));
    assert(false == IsValid1(223450));
    assert(false == IsValid1(123789));
    assert(IsValid2(112233));
    assert(false == IsValid2(123444));
    assert(IsValid2(111122));

    static_assert(HasDoubleDigit(111111));
    static_assert(HasDoubleDigit(112233));
    static_assert(HasDoubleDigit(123444));
    static_assert(HasDoubleDigit(111122));
    static_assert(false == HasDoubleDigitExact(111111));
    static_assert(HasDoubleDigitExact(112233));
    static_assert(false == HasDoubleDigitExact(123444));
    static_assert(HasDoubleDigitExact(111122));

    std::vector<int> loose;
    std::vector<int> strict;

    for (int i = input::begin; i != input::end; ++i)
    {
        if (IsIncreasing(i) == false)
            continue;

        if (HasDoubleDigit(i) == false)
            continue;

        if (HasDoubleDigitExact(i))
            strict.push_back(i);

        loose.push_back(i);
    }

    std::cout << "  Part1: " << loose.size() << '\n';
    std::cout << "  Part2: " << strict.size() << '\n';
}
