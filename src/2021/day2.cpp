#include "day2.hpp"

#include "../cpp-utils.hpp"

static int Part1(std::string_view text)
{
    int pos = 0;
    int depth = 0;

    for (auto const &line : Split(text, '\n'))
    {
        if (starts_with(line, "forward "))
        {
            pos += svtoi(line.substr(8));
        }
        else if (starts_with(line, "up "))
        {
            depth -= svtoi(line.substr(3));
        }
        else if (starts_with(line, "down "))
        {
            depth += svtoi(line.substr(5));
        }
    }

    return pos * depth;
}

static int Part2(std::string_view text)
{
    int pos = 0;
    int depth = 0;
    int aim = 0;

    for (auto const &line : Split(text, '\n'))
    {
        if (starts_with(line, "forward "))
        {
            auto const v = svtoi(line.substr(8));
            pos += v;
            depth += aim * v;
        }
        else if (starts_with(line, "up "))
        {
            auto const v = svtoi(line.substr(3));
            aim -= v;
        }
        else if (starts_with(line, "down "))
        {
            auto const v = svtoi(line.substr(5));
            aim += v;
        }
    }

    return pos * depth;
}

int main()
{
    // https://adventofcode.com/2021/day/2
    std::println("Day 2, 2021: Dive!");

    Assert(150 == Part1(example::directions));
    Assert(900 == Part2(example::directions));

    auto const part1 = Part1(GetInput());
    std::println("  Part 1: {}", part1);
    Assert(1'694'130 == part1);

    auto const part2 = Part2(GetInput());
    std::println("  Part 2: {}", part2);
    Assert(1'698'850'445 == part2);
}
