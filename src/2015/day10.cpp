#include "day10.hpp"
#include "../common/assert.hpp"
#include <fmt/format.h>
#include <gsl/gsl>
#include <sstream>
#include <string>

std::string LookAndSay(std::string_view text)
{
    std::string buffer;
    buffer.reserve(text.size() * 2);
    char prev = text[0];
    text.remove_prefix(1);
    int count = 1;

    for (char c : text)
    {
        if (prev != c)
        {
            buffer.append(std::to_string(count));
            buffer.append(1, prev);
            count = 0;
        }

        prev = c;
        ++count;
    }

    buffer.append(std::to_string(count));
    buffer.append(1, prev);
    return buffer;
}

std::string LookAndSayLoop(int nbLoops)
{
    std::string text{input::data};

    for (int i = 0; i < nbLoops; ++i)
    {
        text = LookAndSay(text);
    }

    return text;
}

int Part1()
{
    return gsl::narrow_cast<int>(LookAndSayLoop(40).size());
}

int Part2()
{
    return gsl::narrow_cast<int>(LookAndSayLoop(50).size());
}

int main()
{
    // https://adventofcode.com/2015/day/10
    fmt::print("Day 10: Elves Look, Elves Say\n");

    Assert("11" == LookAndSay("1"));
    Assert("21" == LookAndSay("11"));
    Assert("1211" == LookAndSay("21"));
    Assert("111221" == LookAndSay("1211"));
    Assert("312211" == LookAndSay("111221"));

    int const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(252594 == part1);

    int const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(3579328 == part2);
}
