#include "day03.hpp"
#include "../common/assert.hpp"
#include <fmt/format.h>
#include <span>
#include <vector>

static unsigned int GetBitCount(std::span<unsigned int const> numbers)
{
    unsigned int nbBits = 0;

    for (auto n : numbers)
    {
        nbBits = std::max(std::bit_width(n), nbBits);
    }

    return nbBits;
}

static bool GetMostPopular(std::span<unsigned int const> numbers, unsigned int bitMask)
{
    size_t count = 0;

    for (unsigned int n : numbers)
    {
        if (n & bitMask)
            ++count;
    }

    if (count * 2 == numbers.size())
        return true;

    return count > numbers.size() / 2;
}

static unsigned int Part1(std::span<unsigned int const> numbers)
{
    unsigned int const nbBits = GetBitCount(numbers);
    unsigned int gamma = 0;
    unsigned int epsilon = 0;

    for (unsigned int i = 0; i < nbBits; ++i)
    {
        unsigned int const bitMask = 1 << i;

        if (GetMostPopular(numbers, bitMask))
            gamma |= bitMask;
        else
            epsilon |= bitMask;
    }

    return gamma * epsilon;
}

enum struct Rating
{
    KeepPopular,
    RemovePopular,
};

static unsigned int GetPart2(std::vector<unsigned int> numbers, Rating rating)
{
    unsigned int const nbBits = GetBitCount(numbers);

    for (unsigned int i = nbBits; i > 0; --i)
    {
        unsigned int const bitMask = 1 << (i - 1);
        auto last = end(numbers);

        bool const popular = GetMostPopular(numbers, bitMask);
        bool const keepPopular = rating == Rating::KeepPopular ? popular : not popular;

        if (keepPopular)
        {
            last = std::remove_if(begin(numbers), end(numbers),
                [bitMask](unsigned int n)
                {
                    return (n & bitMask) == 0;
                });
        }
        else
        {
            last = std::remove_if(begin(numbers), end(numbers),
                [bitMask](unsigned int n)
                {
                    return (n & bitMask) != 0;
                });
        }

        numbers.erase(last, end(numbers));

        if (numbers.size() == 1)
            return numbers.front();
    }

    return 0;
}

static unsigned int Part2(std::vector<unsigned int> numbers)
{
    unsigned int const oxygen = GetPart2(numbers, Rating::KeepPopular);
    unsigned int const co2 = GetPart2(numbers, Rating::RemovePopular);
    return co2 * oxygen;
}

int main()
{
    // https://adventofcode.com/2021/day/3
    fmt::print("Day 3, 2021: Binary Diagnostic\n");

    Assert(198 == Part1(example::numbers));
    Assert(230 == Part2(example::numbers));

    auto const part1 = Part1(input::numbers);
    fmt::print("  Part 1: {}\n", part1);
    Assert(841526 == part1);

    auto const part2 = Part2(input::numbers);
    fmt::print("  Part 2: {}\n", part2);
    Assert(4790390 == part2);
}
