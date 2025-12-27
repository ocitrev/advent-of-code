#include "day14.hpp"

#include "../cpp-utils.hpp"

#include <bit>

static int16_t ToInt16(std::pair<char, char> const &p)
{
    static_assert(sizeof(std::pair<char, char>) == sizeof(int16_t));
    int16_t v;
    memcpy(&v, &p, sizeof(v));
    return v;
}

static int16_t ToInt16(char a, char b)
{
    return ToInt16(std::pair{a, b});
}

struct PolymerTemplate
{
    std::string polymer;
    std::unordered_map<std::string_view, char> mapping;

    void Parse(std::string_view text)
    {
        for (auto line : Split(text, '\n'))
        {
            if (line.empty())
            {
                continue;
            }

            if (polymer.empty())
            {
                polymer = line;
                continue;
            }

            mapping[line.substr(0, 2)] = line.substr(6)[0];
        }
    }

    int64_t Step(int nb)
    {
        std::unordered_map<char, int64_t> letters;

        for (char c : polymer)
        {
            ++letters[c];
        }

        std::unordered_map<int16_t, int64_t> pairs;
        auto const last = std::prev(end(polymer));

        for (auto iter = begin(polymer); iter != last; ++iter)
        {
            auto const a = *iter;
            auto const b = *(iter + 1);
            ++pairs[ToInt16(std::make_pair(a, b))];
        }

        for (int i = 0; i < nb; ++i)
        {
            auto previousPairs = pairs;

            for (auto const &[p, c] : mapping)
            {
                auto const ab = ToInt16(p[0], p[1]);

                if (auto it = previousPairs.find(ab); it != end(previousPairs))
                {
                    auto const ac = ToInt16(p[0], c);
                    auto const cb = ToInt16(c, p[1]);
                    auto const count = it->second;
                    pairs[ab] -= count;
                    pairs[ac] += count;
                    pairs[cb] += count;
                    letters[c] += count;
                }
            }
        }

        Assert(not letters.empty());

        std::vector<int64_t> counts(letters.size());
        std::transform(begin(letters), end(letters), begin(counts),
            [](auto const &elm)
            {
                return elm.second;
            });

        auto const [min, max] = std::ranges::minmax(counts);
        return max - min;
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
    return t.Step(40);
}

int main()
{
    // https://adventofcode.com/2021/day/14
    std::println("Day 14, 2021: Extended Polymerization");

    Assert(1588 == Part1(example::polymer));
    Assert(2'188'189'693'529 == Part2(example::polymer));

    auto const part1 = Part1(GetInput());
    std::println("  Part 1: {}", part1);
    Assert(3587 == part1);

    auto const part2 = Part2(GetInput());
    std::println("  Part 2: {}", part2);
    Assert(3'906'445'077'999 == part2);
}
