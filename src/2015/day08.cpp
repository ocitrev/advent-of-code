#include "day08.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include <fmt/format.h>
#include <string>

std::string Unescape(std::string_view text)
{
    // remove surrounding quotes
    Assert(text.front() == '"');
    Assert(text.back() == '"');
    text.remove_prefix(1);
    text.remove_suffix(1);

    std::string unescaped;
    auto outIt = std::back_inserter(unescaped);

    for (std::size_t i = 0; i < text.size(); ++i)
    {
        char const c = text[i];

        if (c == '\\')
        {
            ++i;
            char const e = text[i];

            if (e == 'x' || e == 'X')
            {
                ++i;
                std::string const hexString(text.substr(i, 2));
                *outIt++ = static_cast<char>(std::stoi(hexString, nullptr, 16));
                ++i;
            }
            else if (e == '"' || e == '\\')
            {
                *outIt++ = e;
            }
            else
            {
                throw std::runtime_error("Invalid escape sequence");
            }
        }
        else
        {
            *outIt++ = c;
        }
    }

    return unescaped;
}

std::string Escape(std::string_view text)
{
    std::string escaped;
    auto outIt = std::back_inserter(escaped);
    *outIt++ = '"';

    for (char c : text)
    {
        if (c == '\\')
        {
            *outIt++ = '\\';
            *outIt++ = '\\';
        }
        else if (c == '"')
        {
            *outIt++ = '\\';
            *outIt++ = '"';
        }
        else
        {
            *outIt++ = c;
        }
    }

    *outIt++ = '"';
    return escaped;
}

int Part1(std::string_view text)
{
    std::size_t total = 0;

    for (auto &&line : Split(text, '\n'))
    {
        total += line.size() - Unescape(line).size();
    }

    return static_cast<int>(total);
}

int Part2(std::string_view text)
{
    std::size_t total = 0;

    for (auto &&line : Split(text, '\n'))
    {
        total += Escape(line).size() - line.size();
    }

    return static_cast<int>(total);
}

int main()
{
    // https://adventofcode.com/2015/day/8
    fmt::print("Day 8: Matchsticks\n");

    Assert(12 == Part1(input::example1));
    Assert(19 == Part2(input::example1));

    int const part1 = Part1(input::data);
    fmt::print("  Part 1: {}\n", part1);
    Assert(1333 == part1);

    int const part2 = Part2(input::data);
    fmt::print("  Part 2: {}\n", part2);
    Assert(2046 == part2);
}
