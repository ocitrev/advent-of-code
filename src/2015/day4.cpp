#include "day4.hpp"
#include "../common/assert.hpp"
#include "../common/md5.hpp"
#include "../common/string.hpp"
#include <array>
#include <fmt/format.h>
#include <vector>

static std::string MineOne(std::string const &key, int n)
{
    MD5 md5;
    md5.Update(key + std::to_string(n));
    return md5.Final();
}

static int Mine(std::string const &key, std::string_view start)
{
    for (int i = 0;; ++i)
    {
        if (std::string const hash = MineOne(key, i); starts_with(hash, start))
        {
            return i;
        }
    }
}

int main()
{
    // https://adventofcode.com/2015/day/4
    fmt::print("Day 4, 2015: The Ideal Stocking Stuffer\n");

    int const part1 = Mine(std::string{GetInput()}, "00000");
    fmt::print("  Part1: {}\n", part1);
    Assert(117946 == part1);

    int const part2 = Mine(std::string{GetInput()}, "000000");
    fmt::print("  Part2: {}\n", part2);
    Assert(3938038 == part2);
}
