#include "day4.hpp"

#include "../cpp-utils.hpp"

static bool IsIncreasing(unsigned int n)
{
    std::string sorted = std::to_string(n);
    std::sort(begin(sorted), end(sorted));
    return n == static_cast<unsigned int>(std::stoi(sorted));
}

constexpr static bool HasDoubleDigit(unsigned int n)
{
    std::array<int, 10> digitCount{};

    do
    {
        if (++digitCount.at(n % 10) > 1)
        {
            return true;
        }

        n /= 10;
    }
    while (n != 0);

    return false;
}

constexpr static bool HasDoubleDigitExact(unsigned int n)
{
    std::array<int, 10> digitCount{};

    do
    {
        ++digitCount.at(n % 10);
        n /= 10;
    }
    while (n != 0);

    for (int i : digitCount) // NOLINT, std::any_of is not constexpr
    {
        if (i == 2)
        {
            return true;
        }
    }

    return false;
}

#ifndef NDEBUG
static bool IsValid1(unsigned int n)
{
    return IsIncreasing(n) && HasDoubleDigit(n);
}

static bool IsValid2(unsigned int n)
{
    return IsIncreasing(n) && HasDoubleDigitExact(n);
}
#endif

static auto ParseInput()
{
    auto const parts = Split(GetInput(), '-');
    return std::make_pair(svtoi<unsigned int>(parts[0]), svtoi<unsigned int>(parts[1]));
}

int main()
{
    std::println("Day 4, 2019: Secure Container");

    Assert(IsIncreasing(111'123));
    Assert(IsIncreasing(135'679));

    Assert(IsValid1(111'111));
    Assert(!IsValid1(223'450));
    Assert(!IsValid1(123'789));
    Assert(IsValid2(112'233));
    Assert(!IsValid2(123'444));
    Assert(IsValid2(111'122));

    static_assert(HasDoubleDigit(111'111));
    static_assert(HasDoubleDigit(112'233));
    static_assert(HasDoubleDigit(123'444));
    static_assert(HasDoubleDigit(111'122));
    static_assert(!HasDoubleDigitExact(111'111));
    static_assert(HasDoubleDigitExact(112'233));
    static_assert(!HasDoubleDigitExact(123'444));
    static_assert(HasDoubleDigitExact(111'122));

    std::vector<unsigned int> loose;
    std::vector<unsigned int> strict;
    auto const [begin, end] = ParseInput();

    for (unsigned int i = begin; i != end; ++i)
    {
        if (!IsIncreasing(i))
        {
            continue;
        }

        if (!HasDoubleDigit(i))
        {
            continue;
        }

        if (HasDoubleDigitExact(i))
        {
            strict.push_back(i);
        }

        loose.push_back(i);
    }

    std::println("  Part1: {}", loose.size());
    Assert(1169 == loose.size());

    std::println("  Part2: {}", strict.size());
    Assert(757 == strict.size());
}
