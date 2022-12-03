#include "day3.hpp"
#include "../common.hpp"
#include <set>

static int GetPriority(char c)
{
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 1;
    
    if (c >= 'A' && c <= 'Z')
        return c - 'A' + 27;

    throw std::invalid_argument("c");
}

static int ParseLine(std::string_view line)
{
    std::vector<char> chars(begin(line), end(line));
    auto first = begin(chars);
    auto mid = first + static_cast<ptrdiff_t>(chars.size() / 2);
    auto last = end(chars);
    std::sort(first, mid);
    std::sort(mid, last);
    char intersection;
    std::set_intersection(first, mid, mid, last, &intersection);
    return GetPriority(intersection);
}

static int PraseLines(std::string_view lines)
{
    int total = 0;

    for (auto line : Split(lines, '\n'))
    {
        total += ParseLine(line);
    }

    return total;
}

static int ParseGroup(std::string_view bag1, std::string_view bag2, std::string_view bag3)
{
    std::set<char> set1(begin(bag1), end(bag1));
    std::set<char> set2(begin(bag2), end(bag2));
    std::set<char> set3(begin(bag3), end(bag3));

    std::set<char> common12;
    std::set<char> common23;
    std::set<char> common13;
    std::set_intersection(begin(set1), end(set1), begin(set2), end(set2), std::inserter(common12, common12.end()));
    std::set_intersection(begin(set2), end(set2), begin(set3), end(set3), std::inserter(common23, common23.end()));
    std::set_intersection(begin(set1), end(set1), begin(set3), end(set3), std::inserter(common13, common13.end()));

    std::unordered_map<char, int> counts;
    
    for (char c : common12)
    {
        ++counts[c];
    }

    for (char c : common23)
    {
        ++counts[c];
    }

    for (char c : common13)
    {
        if (++counts[c] == 3)
            return GetPriority(c);
    }

    return 0;
}

static int ParseGroups(std::string_view lines)
{
    auto const list = Split(lines, '\n');
    
    if (list.size() % 3 != 0)
        throw std::invalid_argument("lines");

    int total = 0;
    
    for (auto iter = begin(list); iter != end(list); iter += 3)
    {
        total += ParseGroup(*iter, *(iter + 1), *(iter + 2));
    }

    return total;
}

static int Part1()
{
    return PraseLines(input::rucksack);
}

static int Part2()
{
    return ParseGroups(input::rucksack);
}

int main()
{
    // https://adventofcode.com/2022/day/3
    fmt::print("Day 3, 2022: Rucksack Reorganization\n");

    Assert(157 == PraseLines(example::rucksack));
    Assert(70 == ParseGroups(example::rucksack));

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(8401 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(2641 == part2);
}