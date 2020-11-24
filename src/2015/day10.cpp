#include "day10.hpp"
#include "../common/assert.hpp"
#include <fmt/format.h>
#include <gsl/gsl>
#include <sstream>

std::string LookAndSay(std::string_view text)
{
    std::string buffer;
    buffer.reserve(text.size() * 2);
    auto prev = begin(text);
    int count = 1;

    for (auto iter = std::next(prev); iter != end(text); ++iter, ++count)
    {
        if (*prev != *iter)
        {
            buffer.append(std::to_string(count));
            buffer.append(1, *prev);
            count = 0;
        }

        prev = iter;
    }

    buffer.append(std::to_string(count));
    buffer.append(1, *prev);
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

    fmt::print("  Part 1: {}\n", Part1());
    fmt::print("  Part 2: {}\n", Part2());
}
