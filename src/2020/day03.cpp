#include "day03.hpp"
#include "../common/assert.hpp"
#include <fmt/format.h>
#include <gsl/gsl>

struct Slope
{
    int right = 1;
    int down = 1;
};

[[nodiscard]] int CountTrees(gsl::span<std::string_view const> map, Slope slope)
{
    std::size_t x = 0;
    std::size_t y = 0;
    int trees = 0;
    std::size_t const mapHeight = map.size();
    std::size_t const mapWidth = map[0].size();

    while (y < mapHeight)
    {
        if (map[y][x] == '#')
            ++trees;

        x = (x + static_cast<std::size_t>(slope.right)) % mapWidth;
        y += static_cast<std::size_t>(slope.down);
    }

    return trees;
}

[[nodiscard]] auto CountTreesInSlopes(gsl::span<std::string_view const> map, gsl::span<Slope const> slopes)
{
    return std::accumulate(begin(slopes), end(slopes), int64_t{1}, [&](auto total, Slope s) {
        return total * CountTrees(map, s);
    });
}

int main()
{
    // https://adventofcode.com/2020/day/3
    fmt::print("Day 3, 2020: \n");

    constexpr std::array allSlopes{Slope{1, 1}, Slope{3, 1}, Slope{5, 1}, Slope{7, 1}, Slope{1, 2}};

    Assert(7 == CountTrees(example::map, Slope{3, 1}));
    Assert(336 == CountTreesInSlopes(example::map, allSlopes));

    auto const part1 = CountTrees(input::map, Slope{3, 1});
    fmt::print("  Part 1: {}\n", part1);
    Assert(232 == part1);

    auto const part2 = CountTreesInSlopes(input::map, allSlopes);
    fmt::print("  Part 2: {}\n", part2);
}
