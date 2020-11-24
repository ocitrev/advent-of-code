#include "day08.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include <fmt/format.h>

std::string Unescape(std::string_view text)
{
    // remove surrounding quotes
    Assert(text.front() == '"');
    Assert(text.back() == '"');
    text.remove_prefix(1);
    text.remove_suffix(1);

    std::string unescaped;
    auto outIt = std::back_inserter(unescaped);

    for (auto iter = begin(text); iter != end(text); ++iter)
    {
        char const c = *iter;

        if (c == '\\')
        {
            ++iter;
            char const e = *iter;

            if (e == 'x' || e == 'X')
            {
                ++iter;
                std::string const hexString(iter, iter + 2);
                *outIt++ = static_cast<char>(std::stoi(hexString, nullptr, 16));
                ++iter;
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

    fmt::print("  Part 1: {}\n", Part1(input::data));
    fmt::print("  Part 1: {}\n", Part2(input::data));
}
