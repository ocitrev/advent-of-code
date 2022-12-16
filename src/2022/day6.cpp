#include "day6.hpp"
#include "../common.hpp"
#include <unordered_set>

int64_t FindMarker(std::string_view data, int64_t len)
{
    auto const first = begin(data);
    auto const last = end(data);
    auto makerstart = first;
    auto markerend = first + len;

    while (true)
    {
        if (markerend == last)
            return -1;

        if (std::unordered_set<char>(makerstart, markerend).size() == static_cast<size_t>(len))
            return std::distance(first, markerend);

        ++makerstart;
        ++markerend;
    }
}

static int64_t Part1()
{
    return FindMarker(GetInput(), 4);
}

static int64_t Part2()
{
    return FindMarker(GetInput(), 14);
}

int main()
{
    // https://adventofcode.com/2022/day/6
    fmt::print("Day 6, 2022: Tuning Trouble\n");

    Assert(7 == FindMarker(example::data1, 4));
    Assert(5 == FindMarker(example::data2, 4));
    Assert(6 == FindMarker(example::data3, 4));
    Assert(10 == FindMarker(example::data4, 4));
    Assert(11 == FindMarker(example::data5, 4));

    Assert(19 == FindMarker(example::data1, 14));
    Assert(23 == FindMarker(example::data2, 14));
    Assert(23 == FindMarker(example::data3, 14));
    Assert(29 == FindMarker(example::data4, 14));
    Assert(26 == FindMarker(example::data5, 14));

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(1953 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(2301 == part2);
}
