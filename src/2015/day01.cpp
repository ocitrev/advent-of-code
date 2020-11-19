#include "day01.hpp"
#include <fmt/format.h>

constexpr int get_depth(std::string_view data)
{
    int depth = 0;

    for (auto c : data)
    {
        if (c == '(')
            ++depth;
        else if (c == ')')
            --depth;
    }

    return depth;
}

constexpr int get_pos_at_depth(std::string_view data, int const depth)
{
    int pos = 0;
    int currentDepth = 0;

    for (auto c : data)
    {
        ++pos;

        if (c == '(')
            ++currentDepth;
        else if (c == ')')
            --currentDepth;

        if (depth == currentDepth)
            return pos;
    }

    return 0;
}

int main()
{
    // https://adventofcode.com/2015/day/1
    fmt::print("Day 1: Not Quite Lisp\n");

    static_assert(0 == get_depth("(())"));
    static_assert(0 == get_depth("()()"));
    static_assert(3 == get_depth("((("));
    static_assert(3 == get_depth("(()(()("));
    static_assert(-1 == get_depth("())"));
    static_assert(-1 == get_depth("))("));
    static_assert(-3 == get_depth(")))"));
    static_assert(-3 == get_depth(")())())"));
    static_assert(1 == get_pos_at_depth(")", -1));
    static_assert(5 == get_pos_at_depth("()())", -1));

    fmt::print("  Part1: {}\n", get_depth(input::data));
    fmt::print("  Part2: {}\n", get_pos_at_depth(input::data, -1));
}