#include "day05.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include <array>
#include <fmt/format.h>
#include <map>

static bool IsNice1(std::string_view text)
{
    if (text.find("ab") != std::string_view::npos)
        return false;

    if (text.find("cd") != std::string_view::npos)
        return false;

    if (text.find("pq") != std::string_view::npos)
        return false;

    if (text.find("xy") != std::string_view::npos)
        return false;

    int vowels = 0;
    char last = '\0';
    bool hasDouble = false;

    for (char c : text)
    {
        if (c == last)
            hasDouble = true;

        switch (c)
        {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
            ++vowels;
            break;
        }

        if (hasDouble && vowels >= 3)
            return true;

        last = c;
    }

    return false;
}

static int Part1()
{
    int count = 0;

    for (std::string const &line : Split(input::data, '\n'))
    {
        if (IsNice1(line))
            ++count;
    }

    return count;
}

static bool HasRepeatingPair(std::string_view text)
{
    std::map<std::pair<char, char>, size_t> pairs;
    char last = text[0];
    size_t offset = 0;

    for (char c : text.substr(1))
    {
        auto const pair = std::make_pair(last, c);
        auto [it, inserted] = pairs.insert(std::make_pair(pair, offset));

        // gcc wrongly warns about null dereference here
        if (it == end(pairs))
            continue;

        if (not inserted && it->second != offset - 1)
        {
            return true;
        }

        ++offset;
        last = c;
    }

    return false;
}

static bool HasSandwich(std::string_view text)
{
    char last0 = text[0];
    char last1 = text[1];

    for (char c : text.substr(2))
    {
        if (last0 == c)
        {
            return true;
        }

        last0 = std::exchange(last1, c);
    }

    return false;
}

static bool IsNice2(std::string_view text)
{
    return HasRepeatingPair(text) && HasSandwich(text);
}

static int Part2()
{
    int count = 0;

    for (std::string const &line : Split(input::data, '\n'))
    {
        if (IsNice2(line))
            ++count;
    }

    return count;
}

int main()
{
    // https://adventofcode.com/2015/day/5
    fmt::print("Day 5: Doesn't He Have Intern-Elves For This?\n");

    Assert(IsNice1("ugknbfddgicrmopn"));
    Assert(IsNice1("aaa"));
    Assert(!IsNice1("jchzalrnumimnmhp"));
    Assert(!IsNice1("haegwjzuvuyypxyu"));
    Assert(!IsNice1("dvszwmarrgswjxmb"));
    Assert(IsNice2("qjhvhtzxzqqjkmpb"));
    Assert(IsNice2("xxyxx"));
    Assert(!IsNice2("uurcxstgmygtbstg"));
    Assert(!IsNice2("ieodomkazucvgmuy"));

    int const part1 = Part1();
    fmt::print("  Part1: {}\n", part1);
    Assert(238 == part1);

    int const part2 = Part2();
    fmt::print("  Part2: {}\n", part2);
    Assert(69 == part2);
}
