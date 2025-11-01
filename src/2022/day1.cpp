#include "day1.hpp"

#include "../common.hpp"

#include <vector>

static auto Part1(std::string_view lines)
{
    int64_t calories = 0;
    int64_t biggest = 0;

    for (auto line : Split(lines, '\n'))
    {
        if (line.empty())
        {
            biggest = std::max(biggest, std::exchange(calories, 0));
        }

        calories += svtoi<int64_t>(line);
    }

    return std::max(biggest, calories);
}

static auto Part2(std::string_view lines)
{
    std::vector<int64_t> elves;
    int64_t calories = 0;

    for (auto line : Split(lines, '\n'))
    {
        if (line.empty())
        {
            elves.push_back(std::exchange(calories, 0));
        }

        calories += svtoi<int64_t>(line);
    }

    elves.push_back(calories);
    std::partial_sort(begin(elves), begin(elves) + 3, end(elves), std::greater{});
    return std::accumulate(begin(elves), begin(elves) + 3, 0);
}

int main()
{
    // https://adventofcode.com/2022/day/1
    fmt::print("Day 1, 2022: Calorie Counting\n");

    Assert(24'000 == Part1(example::lines));
    Assert(45'000 == Part2(example::lines));

    auto const part1 = Part1(GetInput());
    fmt::print("  Part 1: {}\n", part1);
    Assert(65'912 == part1);

    auto const part2 = Part2(GetInput());
    fmt::print("  Part 2: {}\n", part2);
    Assert(195'625 == part2);
}
