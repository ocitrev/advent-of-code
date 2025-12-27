#include "day1.hpp"

#include "../cpp-utils.hpp"

#include <array>
#include <optional>
#include <span>

static std::vector<int> ParseNumbers(std::string_view lines)
{
    std::vector<int> numbers;

    for (auto line : Split(lines, '\n'))
    {
        numbers.push_back(svtoi(line));
    }

    return numbers;
}

static int Part1(std::span<int const> values)
{
    std::optional<int> prev;
    int nbIncrease = 0;

    for (auto v : values)
    {
        if (prev.has_value() && v > *prev)
        {
            ++nbIncrease;
        }

        prev = v;
    }

    return nbIncrease;
}

static int Part2(std::span<int const> values)
{
    auto windowStart = begin(values);
    auto windowEnd = next(windowStart, 3);
    auto const last = end(values);

    int prev = std::accumulate(windowStart++, windowEnd++, 0);
    int nbIncrease = 0;

    for (;; ++windowStart, ++windowEnd)
    {
        auto const v = std::accumulate(windowStart, windowEnd, 0);

        if (v > prev)
        {
            ++nbIncrease;
        }

        prev = v;

        if (windowEnd == last)
        {
            break;
        }
    }

    return nbIncrease;
}

int main()
{
    // https://adventofcode.com/2021/day/1
    std::println("Day 1, 2021: Sonar Sweep");

    Assert(7 == Part1(example::numbers));
    Assert(5 == Part2(example::numbers));

    auto const part1 = Part1(ParseNumbers(GetInput()));
    std::println("  Part 1: {}", part1);
    Assert(1316 == part1);

    auto const part2 = Part2(ParseNumbers(GetInput()));
    std::println("  Part 2: {}", part2);
    Assert(1344 == part2);
}
