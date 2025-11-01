#include "day6.hpp"

#include "../common/assert.hpp"
#include "../common/string.hpp"

#include <algorithm>
#include <fmt/format.h>
#include <iterator>
#include <set>

[[nodiscard]] static int GetCount1(std::string_view group)
{
    std::set<char> responses;

    for (char c : group)
    {
        if (c == ' ' || c == '\n')
        {
            continue;
        }

        responses.insert(c);
    }

    return static_cast<int>(responses.size());
}

[[nodiscard]] static int GetSum1(std::string_view answers)
{
    int total = 0;

    for (auto &&group : Split(answers, "\n\n"))
    {
        total += GetCount1(group);
    }

    return total;
}

[[nodiscard]] static int GetCount2(std::string_view group)
{
    std::set<char> prev;
    bool first = true;

    for (auto &&line : Split(group, "\n"))
    {
        if (first)
        {
            first = false;
            std::copy(begin(line), end(line), std::inserter(prev, end(prev)));
            continue;
        }

        std::set<char> current;
        std::copy(begin(line), end(line), std::inserter(current, end(current)));

        std::set<char> intersection;
        std::set_intersection(begin(prev), end(prev), begin(current), end(current),
            std::inserter(intersection, end(intersection)));

        if (intersection.empty())
        {
            return 0;
        }

        prev = std::move(intersection);
    }

    return static_cast<int>(prev.size());
}

[[nodiscard]] static int GetSum2(std::string_view answers)
{
    int total = 0;
    for (auto &&group : Split(answers, "\n\n"))
    {
        total += GetCount2(group);
    }

    return total;
}

int main()
{
    // https://adventofcode.com/2020/day/6
    fmt::print("Day 6, 2020: Custom Customs\n");

    Assert(11 == GetSum1(example::answers));
    Assert(6 == GetSum2(example::answers));

    auto const part1 = GetSum1(GetInput());
    fmt::print("  Part 1: {}\n", part1);
    Assert(6625 == part1);

    auto const part2 = GetSum2(GetInput());
    fmt::print("  Part 2: {}\n", part2);
    Assert(3360 == part2);
}
