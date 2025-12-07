#include "day1.hpp"

#include "../common.hpp"

static constexpr int kSumWanted = 2020;

static int Find(std::span<int const> numbers, std::size_t nb)
{
    int result = 0;
    Combinations(numbers, nb,
        [&](auto const &values)
        {
            if (kSumWanted == std::accumulate(begin(values), end(values), 0, std::plus{}))
            {
                result = std::accumulate(begin(values), end(values), 1, std::multiplies{});
                return false; // break
            }

            return true; // continue
        });

    return result;
}

static void Example()
{
    Assert(514'579 == Find(example::numbers, 2));
    Assert(241'861'950 == Find(example::numbers, 3));
}

static int Part1()
{
    return Find(ParseInputNumbers(), 2);
}

static int Part2()
{
    return Find(ParseInputNumbers(), 3);
}

int main()
{
    // https://adventofcode.com/2020/day/1
    std::print("Day 1, 2020: Report Repair\n");

    Example();

    auto const part1 = Part1();
    std::print("  Part 1: {}\n", part1);
    Assert(806'656 == part1);

    auto const part2 = Part2();
    std::print("  Part 2: {}\n", part2);
    Assert(230'608'320 == part2);
}
