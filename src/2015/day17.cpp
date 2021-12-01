#include "day17.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include "../common/utils.hpp"
#include <fmt/format.h>

static auto HowManyCombinationsCanFit(std::vector<int> const &input, int limit)
{
    int nbCombinationsTotal = 0;
    int nbCombinations = 0;

    auto const nb = input.size();

    for (std::size_t i = 1; i < nb; ++i)
    {
        bool found = false;
        Combinations(gsl::span{input}, i,
                     [&](auto const &values)
                     {
                         if (std::accumulate(begin(values), end(values), 0) == limit)
                         {
                             ++nbCombinationsTotal;
                             found = true;
                         }
                         return true;
                     });

        if (nbCombinations == 0)
            nbCombinations = nbCombinationsTotal;

        if (nbCombinationsTotal != 0 && not found)
            break;
    }

    return std::make_pair(nbCombinationsTotal, nbCombinations);
}

void Example()
{
    Assert(std::make_pair(4, 3) == HowManyCombinationsCanFit({20, 15, 10, 5, 5}, 25));
}

static auto Parts()
{
    std::vector<int> containers;

    for (auto &&line : Split(input::text, '\n'))
    {
        containers.emplace_back(svtoi(line));
    }

    return HowManyCombinationsCanFit(containers, 150);
}

int main()
{
    // https://adventofcode.com/2015/day/17
    fmt::print("Day 17, 2015: No Such Thing as Too Much\n");

    Example();

    auto const [part1, part2] = Parts();
    fmt::print("  Part 1: {}\n", part1);
    Assert(1304 == part1);

    fmt::print("  Part 2: {}\n", part2);
    Assert(18 == part2);
}
