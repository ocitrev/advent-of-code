#include "day04.hpp"
#include "../common/md5.hpp"
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
        if (std::string const hash = MineOne(key, i); hash.starts_with(start))
        {
            return i;
        }
    }
}

int main()
{
    // https://adventofcode.com/2015/day/4
    fmt::print("Day 4: The Ideal Stocking Stuffer\n");

    fmt::print("  Part1: {}\n", Mine(std::string{input::data}, "00000"));
    fmt::print("  Part2: {}\n", Mine(std::string{input::data}, "000000"));
}
