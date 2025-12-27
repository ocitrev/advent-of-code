#include "day6.hpp"

#include "../cpp-utils.hpp"

static std::vector<int8_t> ParseInput()
{
    std::vector<int8_t> numbers;

    for (auto number : Split(GetInput(), ','))
    {
        numbers.push_back(svtoi<int8_t>(number));
    }

    return numbers;
}
static uint64_t Run(std::span<int8_t const> fishes, int const days)
{
    std::array<uint64_t, 9> counters{};

    for (int8_t f : fishes)
    {
        ++counters[static_cast<size_t>(f)];
    }

    for (int d = 0; d != days; ++d)
    {
        std::rotate(begin(counters), begin(counters) + 1, end(counters));
        counters[6] += counters[8];
    }

    return std::accumulate(begin(counters), end(counters), uint64_t{0});
}

static uint64_t Part1(std::span<int8_t const> fishes)
{
    return Run(fishes, 80);
}

static uint64_t Part2(std::span<int8_t const> fishes)
{
    return Run(fishes, 256);
}

int main()
{
    // https://adventofcode.com/2021/day/6
    std::println("Day 6, 2021: Lanternfish");

    Assert(26 == Run(example::fishes, 18));
    Assert(5934 == Run(example::fishes, 80));
    Assert(26'984'457'539 == Run(example::fishes, 256));

    auto const part1 = Part1(ParseInput());
    std::println("  Part 1: {}", part1);
    Assert(359'344 == part1);

    auto const part2 = Part2(ParseInput());
    std::println("  Part 2: {}", part2);
    Assert(1'629'570'219'571 == part2);
}
