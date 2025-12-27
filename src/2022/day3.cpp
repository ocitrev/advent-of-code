#include "day3.hpp"

#include "../common.hpp"

#include <set>

static int GetPriority(char c)
{
    if (c >= 'a' && c <= 'z')
    {
        return c - 'a' + 1;
    }

    if (c >= 'A' && c <= 'Z')
    {
        return c - 'A' + 27;
    }

    throw std::invalid_argument("c");
}

static int ParseLine(std::string_view line)
{
    auto const first = begin(line);
    auto const mid = first + static_cast<ptrdiff_t>(line.size() / 2);
    auto const last = end(line);
    std::set<char> const half1(first, mid);
    std::set<char> const half2(mid, last);
    char intersection;
    std::set_intersection(begin(half1), end(half1), begin(half2), end(half2), &intersection);
    return GetPriority(intersection);
}

static int PraseLines(std::string_view lines)
{
    return TransformReduceLines(lines, 0, std::plus{}, &ParseLine);
}

static int ParseGroup(std::string_view bag1, std::string_view bag2, std::string_view bag3)
{
    std::set<char> const set1(begin(bag1), end(bag1));
    std::set<char> const set2(begin(bag2), end(bag2));
    std::set<char> const set3(begin(bag3), end(bag3));
    std::set<char> intermediate;
    char intersection;
    std::set_intersection(begin(set1), end(set1), begin(set2), end(set2), std::inserter(intermediate, intermediate.end()));
    std::set_intersection(begin(intermediate), end(intermediate), begin(set3), end(set3), &intersection);
    return GetPriority(intersection);
}

static int ParseGroups(std::string_view lines)
{
    auto const list = Split(lines, '\n');

    if (list.size() % 3 != 0)
    {
        throw std::invalid_argument("lines");
    }

    int total = 0;

    for (auto iter = begin(list); iter != end(list); iter += 3)
    {
        total += ParseGroup(*iter, *(iter + 1), *(iter + 2));
    }

    return total;
}

static int Part1()
{
    return PraseLines(GetInput());
}

static int Part2()
{
    return ParseGroups(GetInput());
}

int main()
{
    // https://adventofcode.com/2022/day/3
    std::println("Day 3, 2022: Rucksack Reorganization");

    Assert(157 == PraseLines(example::rucksack));
    Assert(70 == ParseGroups(example::rucksack));

    auto const part1 = Part1();
    std::println("  Part 1: {}", part1);
    Assert(8401 == part1);

    auto const part2 = Part2();
    std::println("  Part 2: {}", part2);
    Assert(2641 == part2);
}
