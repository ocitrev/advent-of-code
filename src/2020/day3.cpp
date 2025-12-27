#include "day3.hpp"

#include "../common/assert.hpp"
#include "../common/string.hpp"

#include <array>
#include <numeric>
#include <print>
#include <span>

struct Slope
{
    int right = 1;
    int down = 1;
};

[[nodiscard]] static int CountTrees(std::span<std::string_view const> map, Slope slope)
{
    std::size_t x = 0;
    std::size_t y = 0;
    int trees = 0;
    std::size_t const mapHeight = map.size();
    std::size_t const mapWidth = map[0].size();

    while (y < mapHeight)
    {
        if (map[y][x] == '#')
        {
            ++trees;
        }

        x = (x + static_cast<std::size_t>(slope.right)) % mapWidth;
        y += static_cast<std::size_t>(slope.down);
    }

    return trees;
}

[[nodiscard]] static int CountTrees(std::string_view map, Slope slope)
{
    auto const lines = Split(map, '\n');
    return CountTrees(lines, slope);
}

[[nodiscard]] static auto CountTreesInSlopes(std::span<std::string_view const> map, std::span<Slope const> slopes)
{
    return std::accumulate(begin(slopes), end(slopes), int64_t{1},
        [&](auto total, Slope s)
        {
            return total * CountTrees(map, s);
        });
}

[[nodiscard]] static auto CountTreesInSlopes(std::string_view map, std::span<Slope const> slopes)
{
    auto const lines = Split(map, '\n');
    return CountTreesInSlopes(lines, slopes);
}

int main()
{
    // https://adventofcode.com/2020/day/3
    std::println("Day 3, 2020: ");

    constexpr std::array allSlopes{Slope{1, 1}, Slope{3, 1}, Slope{5, 1}, Slope{7, 1}, Slope{1, 2}};

    Assert(7 == CountTrees(example::map, Slope{3, 1}));
    Assert(336 == CountTreesInSlopes(example::map, allSlopes));

    auto const part1 = CountTrees(GetInput(), Slope{3, 1});
    std::println("  Part 1: {}", part1);
    Assert(232 == part1);

    auto const part2 = CountTreesInSlopes(GetInput(), allSlopes);
    std::println("  Part 2: {}", part2);
    Assert(3'952'291'680 == part2);
}
