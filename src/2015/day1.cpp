#include "day1.hpp"

#include "../common.hpp"

static constexpr int get_depth(std::string_view data)
{
    int depth = 0;

    for (auto c : data)
    {
        if (c == '(')
        {
            ++depth;
        }
        else if (c == ')')
        {
            --depth;
        }
    }

    return depth;
}

static constexpr int get_pos_at_depth(std::string_view data, int const depth)
{
    int pos = 0;
    int currentDepth = 0;

    for (auto c : data)
    {
        ++pos;

        if (c == '(')
        {
            ++currentDepth;
        }
        else if (c == ')')
        {
            --currentDepth;
        }

        if (depth == currentDepth)
        {
            return pos;
        }
    }

    return 0;
}

int main()
{
    // https://adventofcode.com/2015/day/1
    std::println("Day 1, 2015: Not Quite Lisp");

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

    constexpr int part1 = get_depth(GetInput());
    static_assert(232 == part1);
    std::println("  Part1: {}", part1);

    constexpr int part2 = get_pos_at_depth(GetInput(), -1);
    static_assert(1783 == part2);
    std::println("  Part2: {}", part2);
}
