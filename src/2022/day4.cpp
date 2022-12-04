#include "day4.hpp"
#include "../common.hpp"

struct Range
{
    int low;
    int high;

    bool FullOverlap(Range other) const
    {
        if (low >= other.low && high <= other.high)
            return true;

        if (other.low >= low && other.high <= high)
            return true;

        return false;
    }

    bool NoOverlap(Range other) const
    {
        if (high < other.low || low > other.high)
            return true;

        if (other.high < low || other.low > high)
            return true;

        return false;
    }
};

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
    return CountFullOverlap(input::pairs);
}

static int Part2()
{
    return CountAnyOverlap(input::pairs);
}

int main()
{
    // https://adventofcode.com/2022/day/4
    fmt::print("Day 4, 2022: Camp Cleanup\n");

    Assert(2 == CountFullOverlap(example::pairs));
    Assert(4 == CountAnyOverlap(example::pairs));

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(424 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    // Assert( == part2);
}