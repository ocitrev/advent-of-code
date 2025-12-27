#include "day4.hpp"

#include "../common.hpp"
#include "../common/range.hpp"

static Range ParseRange(std::string_view range)
{
    auto const parts = Split(range, '-');
    return {svtoi(parts[0]), svtoi(parts[1])};
}

static std::pair<Range, Range> ParseRanges(std::string_view ranges)
{
    auto const elves = Split(ranges, ',');
    return {ParseRange(elves[0]), ParseRange(elves[1])};
}

static bool FullOverlap(std::string_view pair)
{
    auto const [elve1, elve2] = ParseRanges(pair);
    return elve1.FullOverlap(elve2);
}

static bool AnyOverlap(std::string_view pair)
{
    auto const [elve1, elve2] = ParseRanges(pair);
    return not elve1.NoOverlap(elve2);
}

static int CountFullOverlap(std::string_view pairs)
{
    return TransformReduceLines(pairs, 0, std::plus{}, &FullOverlap);
}

static int CountAnyOverlap(std::string_view pairs)
{
    return TransformReduceLines(pairs, 0, std::plus{}, &AnyOverlap);
}

static int Part1()
{
    return CountFullOverlap(GetInput());
}

static int Part2()
{
    return CountAnyOverlap(GetInput());
}

int main()
{
    // https://adventofcode.com/2022/day/4
    std::println("Day 4, 2022: Camp Cleanup");

    Assert(2 == CountFullOverlap(example::pairs));
    Assert(4 == CountAnyOverlap(example::pairs));

    auto const part1 = Part1();
    std::println("  Part 1: {}", part1);
    Assert(424 == part1);

    auto const part2 = Part2();
    std::println("  Part 2: {}", part2);
    Assert(804 == part2);
}
