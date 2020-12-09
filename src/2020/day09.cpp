#include "day09.hpp"
#include "../common/assert.hpp"
#include "../common/utils.hpp"
#include <fmt/format.h>
#include <gsl/gsl>
#include <vector>

int64_t FindOutlier(gsl::span<int64_t const> numbers, int preambleLength)
{
    auto preambleFirst = begin(numbers);
    auto preambleLast = preambleFirst + preambleLength;

    for (auto iter = preambleLast; iter != end(numbers); ++iter, ++preambleFirst, ++preambleLast)
    {
        auto const n = *iter;
        bool isOk = false;
        Combinations(gsl::span{&*preambleFirst, &*preambleLast}, 2, [n, &isOk](auto const &values) {
            if (n == values[0] + values[1])
            {
                isOk = true;
                return false;
            }

            return true;
        });

        if (not isOk)
        {
            return n;
        }
    }

    return 0;
}

int64_t Part2(gsl::span<int64_t const> numbers, int64_t weak)
{
    int len = 1;
    int const total = static_cast<int>(numbers.size());

    while (++len <= total)
    {
        auto first = begin(numbers);
        auto last = first + len;

        while (true)
        {
            if (weak == std::accumulate(first, last, int64_t{0}))
            {
                auto const [min, max] = std::minmax_element(first, last);
                return *min + *max;
            }

            if (last == end(numbers))
                break;

            ++first;
            ++last;
        }
    }

    return 0;
}

int main()
{
    // https://adventofcode.com/2020/day/9
    fmt::print("Day 9: Encoding Error\n");

    auto const part1 = FindOutlier(input::numbers, input::preamble);
    fmt::print("  Part 1: {}\n", part1);
    Assert(14360655 == part1);

    auto const part2 = Part2(input::numbers, part1);
    fmt::print("  Part 2: {}\n", part2);
    Assert(1962331 == part2);
}
