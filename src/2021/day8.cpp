#include "day8.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include "../common/utils.hpp"
#include <algorithm>
#include <array>
#include <fmt/format.h>
#include <numeric>
#include <set>

static int CountSimple(std::string_view signalPatterns)
{
    int count = 0;

    for (auto pattern : Split(signalPatterns, ' '))
    {
        auto const size = pattern.size();

        if (size == 2 || size == 3 || size == 4 || size == 7)
        {
            ++count;
        }
    }

    return count;
}

static int Part1(std::string_view entries)
{
    int count = 0;

    for (auto line : Split(entries, '\n'))
    {
        auto const parts = Split(line, '|');
        count += CountSimple(trim_copy(parts[1]));
    }

    return count;
}

static std::unordered_map<std::string, int> Analyze(std::string_view signalPatterns)
{
    auto patterns = Split(signalPatterns, ' ');
    std::sort(begin(patterns), end(patterns),
        [](auto const &a, auto const &b)
        {
            auto const as = a.size();
            auto const bs = b.size();

            if (as == bs)
                return a < b;

            return as < bs;
        });

    std::vector<std::string> sortedPatterns(patterns.size());
    std::transform(begin(patterns), end(patterns), begin(sortedPatterns),
        [](std::string_view const &s)
        {
            std::string p{s};
            std::sort(begin(p), end(p));
            return p;
        });

    std::unordered_map<std::string, int> mapping;
    std::array<std::string, 10> digits;

    for (auto p : sortedPatterns)
    {
        auto const s = p.size();

        if (s == 2)
        {
            mapping[p] = 1;
            digits[1] = p;
        }
        else if (s == 3)
        {
            mapping[p] = 7;
            digits[7] = p;
        }
        else if (s == 4)
        {
            mapping[p] = 4;
            digits[4] = p;
        }
        else if (s == 7)
        {
            mapping[p] = 8;
            digits[8] = p;
        }
    }

    char topLeft = '\0';
    char middle = '\0';

    for (auto p : sortedPatterns)
    {
        auto const s = p.size();

        if (s == 5)
        {
            // 3 doit contenir tous les digits de 1 contrairement a 2 et 5
            if (std::all_of(begin(digits[1]), end(digits[1]),
                    [&p](char c)
                    {
                        return p.find(c) != std::string::npos;
                    }))
            {
                mapping[p] = 3;
                digits[3] = p;

                // trouve le segment de 4 qui n'est pas dans 3
                std::set_difference(begin(digits[4]), end(digits[4]), begin(p), end(p), &topLeft);

                // trouve le segment de 4 qui n'est pas dans 7, moins le segment precedant
                std::set<char> tmp;
                std::set_difference(
                    begin(digits[4]), end(digits[4]), begin(digits[7]), end(digits[7]), std::inserter(tmp, tmp.end()));
                tmp.erase(topLeft);
                middle = *begin(tmp);
            }
        }
        else if (s == 6)
        {
            // 6 ne doit pas contenir tous les digits de 7 contrairement a 0 et 9
            if (not std::all_of(begin(digits[7]), end(digits[7]),
                    [&p](char c)
                    {
                        return p.find(c) != std::string::npos;
                    }))
            {
                mapping[p] = 6;
                digits[6] = p;
            }
        }
    }

    for (auto p : sortedPatterns)
    {
        auto const s = p.size();

        if (s == 5 && digits[3] != p)
        {
            mapping[p] = p.find(topLeft) == std::string::npos ? 2 : 5;
        }
        if (s == 6 && digits[6] != p)
        {
            mapping[p] = p.find(middle) == std::string::npos ? 0 : 9;
        }
    }

    return mapping;
}

static int MapNumber(auto const &mapping, std::string_view text)
{
    int result = 0;
    int mult = 1000;

    for (auto const &n : Split(text, ' '))
    {
        std::string sorted_n{n};
        std::sort(begin(sorted_n), end(sorted_n));
        result += mapping.find(sorted_n)->second * mult;
        mult /= 10;
    }

    return result;
}

static int Part2(std::string_view entries)
{
    int total = 0;

    for (auto line : Split(entries, '\n'))
    {
        auto const parts = Split(line, '|');
        auto const mapping = Analyze(trim_copy(parts[0]));
        total += MapNumber(mapping, trim_copy(parts[1]));
    }

    return total;
}

int main()
{
    // https://adventofcode.com/2021/day/8
    fmt::print("Day 8, 2021: Seven Segment Search\n");

    Assert(26 == Part1(example::entries));
    Assert(61229 == Part2(example::entries));

    auto const part1 = Part1(input::entries);
    fmt::print("  Part 1: {}\n", part1);
    Assert(392 == part1);

    auto const part2 = Part2(input::entries);
    fmt::print("  Part 2: {}\n", part2);
    Assert(1004688 == part2);
}
