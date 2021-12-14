#include "day14.hpp"
#include "../common.hpp"

struct PolymerTemplate
{
    std::string polymer;
    std::unordered_map<std::string_view, char> mapping;

    void Parse(std::string_view text)
    {
        for (auto line : Split(text, '\n'))
        {
            if (line.empty())
                continue;

            if (polymer.empty())
            {
                polymer = line;
                continue;
            }

            mapping[line.substr(0, 2)] = line.substr(6)[0];
        }
    }

    void StepOnce()
    {
        auto const last = std::prev(end(polymer));
        std::string result;
        result.reserve(polymer.size() * 2);

        for (auto iter = begin(polymer); iter != last; ++iter)
        {
            std::string_view pair{iter, iter + 2};

            result.push_back(pair[0]);
            if (auto it = mapping.find(pair); it != end(mapping))
                result.push_back(it->second);
        }

        result.push_back(polymer.back());
        polymer = std::move(result);
    }

    size_t Step(int nb)
    {
        for (int i = 0; i < nb; ++i)
        {
            StepOnce();
        }

        std::unordered_map<char, size_t> counts{};
        for (char c : polymer)
        {
            ++counts[c];
        }

        auto [min, max] = std::ranges::minmax_element(counts,
            [](auto const &a, auto const &b)
            {
                return a.second < b.second;
            });

        if (min == end(counts) || max == end(counts))
            return 0;

        return max->second - min->second;
    }
};

static auto Part1(std::string_view text)
{
    PolymerTemplate t;
    t.Parse(text);
    return t.Step(10);
}

static auto Part2(std::string_view text)
{
    PolymerTemplate t;
    t.Parse(text);
    return t.Step(31);
}

int main()
{
    // https://adventofcode.com/2021/day/14
    fmt::print("Day 14, 2021: Extended Polymerization\n");

    Assert(1588 == Part1(example::polymer));

    auto const part1 = Part1(input::polymer);
    fmt::print("  Part 1: {}\n", part1);
    Assert(3587 == part1);

    auto const part2 = Part2(example::polymer);
    fmt::print("  Part 2: {}\n", part2);
    // Assert( == part2);
}
