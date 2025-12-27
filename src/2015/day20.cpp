#include "day20.hpp"

#include "../common/assert.hpp"
#include "../common/string.hpp"

#include <print>
#include <vector>

int Part1(int limit)
{
    // la somme des diviseurs est trop lente.
    int const size = limit / 10;
    std::vector<int> giftsPerHouse(static_cast<std::size_t>(size), 10);

    for (int elve = 2; elve < size; ++elve)
    {
        for (int i = elve; i < size; i += elve)
        {
            giftsPerHouse[static_cast<std::size_t>(i)] += 10 * elve;
        }
    }

    auto iter = std::find_if(begin(giftsPerHouse), end(giftsPerHouse),
        [limit](int i)
        {
            return i >= limit;
        });

    return static_cast<int>(std::distance(begin(giftsPerHouse), iter));
}

int Part2(int limit)
{
    int const size = limit / 10;
    std::vector<int> giftsPerHouse(static_cast<std::size_t>(size));

    for (int elve = 1; elve < size; ++elve)
    {
        int visited = 0;

        for (int i = elve; i < size; i += elve)
        {
            giftsPerHouse[static_cast<std::size_t>(i)] += 11 * elve;

            if (++visited == 50)
            {
                break;
            }
        }
    }

    auto iter = std::find_if(begin(giftsPerHouse), end(giftsPerHouse),
        [limit](int i)
        {
            return i >= limit;
        });

    return static_cast<int>(std::distance(begin(giftsPerHouse), iter));
}

static int ParseInput()
{
    return svtoi(GetInput());
}

int main()
{
    // https://adventofcode.com/2015/day/20
    std::println("Day 20, 2015: Infinite Elves and Infinite Houses");

    Assert(2 == Part1(30));
    Assert(3 == Part1(40));
    Assert(4 == Part1(70));
    Assert(6 == Part1(120));

    auto const part1 = Part1(ParseInput());
    std::println("  Part 1: {}", part1);
    Assert(786'240 == part1);

    auto const part2 = Part2(ParseInput());
    std::println("  Part 2: {}", part2);
    Assert(831'600 == part2);
}
