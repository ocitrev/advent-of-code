#include "day24.hpp"

#include "../common/assert.hpp"
#include "../common/string.hpp"
#include "../common/utils.hpp"

#include <algorithm>
#include <fmt/format.h>
#include <gsl/gsl>
#include <iterator>
#include <span>
#include <vector>

static std::vector<std::vector<int>> FindGroups(std::span<int const> ints, int nbPackages)
{
    auto const max = ints.size();
    int const groupSum = std::accumulate(begin(ints), end(ints), 0) / nbPackages;
    std::vector<std::vector<int>> groups;

    for (size_t i = 1; i < max; ++i)
    {
        size_t const oldSize = groups.size();
        Combinations(ints, i,
            [&](auto const &values)
            {
                if (std::accumulate(begin(values), end(values), 0) == groupSum)
                {
                    groups.push_back(values);
                }

                return true;
            });

        if (not groups.empty() && groups.size() == oldSize)
        {
            break;
        }
    }

    return groups;
}

static_assert(std::is_same_v<std::size_t, size_t>);

int64_t GetBestFit(std::span<int const> numbers, int nbPackages)
{
    auto groups = FindGroups(numbers, nbPackages);

#if DEBUG_PRINT
    for (auto &&g : groups)
    {
        auto list = Join(begin(g), end(g), " ",
            [](auto v)
            {
                return std::to_string(v);
            });
        fmt::print("{:<12} (QE={:3})\n", list, std::accumulate(begin(g), end(g), 1LL, std::multiplies{}));
    }
#endif

    std::vector<std::pair<int, int64_t>> groupWithQE;
    std::transform(begin(groups), end(groups), std::back_inserter(groupWithQE),
        [](auto const &g)
        {
            return std::make_pair(static_cast<int>(g.size()), std::accumulate(begin(g), end(g), 1LL, std::multiplies{}));
        });

    Ensures(not groupWithQE.empty());
    auto best = std::min_element(begin(groupWithQE), end(groupWithQE));
    return best->second;
}

static std::vector<int> ParseInput()
{
    std::vector<int> numbers;

    for (auto number : Split(GetInput(), '\n'))
    {
        numbers.push_back(svtoi(number));
    }

    return numbers;
}

int main()
{
    // https://adventofcode.com/2015/day/24
    fmt::print("Day 24, 2015: It Hangs in the Balance\n");

    Assert(99 == GetBestFit(example::numbers, 3));
    Assert(44 == GetBestFit(example::numbers, 4));

    auto const part1 = GetBestFit(ParseInput(), 3);
    fmt::print("  Part 1: {}\n", part1);
    Assert(11'266'889'531 == part1);

    auto const part2 = GetBestFit(ParseInput(), 4);
    fmt::print("  Part 2: {}\n", part2);
    Assert(77'387'711 == part2);
}
