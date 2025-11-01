#include "day8.hpp"

#include "../common.hpp"

#include <array>
#include <bitset>
#include <set>

static int CountSimple(std::string_view signalPatterns)
{
    int count = 0;

    for (auto pattern : Split(signalPatterns, ' '))
    {
        auto const size = pattern.size();

        if (size == 2 || size == 3 || size == 4 || size == 7)
        {
            ++count;
        }
    }

    return count;
}

static int Part1(std::string_view entries)
{
    int count = 0;

    for (auto line : Split(entries, '\n'))
    {
        auto const parts = Split(line, '|');
        count += CountSimple(trim_copy(parts[1]));
    }

    return count;
}

using Segments = std::bitset<7>;

static Segments ToSegments(std::string_view text)
{
    Segments result;

    for (char c : text)
    {
        result[static_cast<size_t>(c - 'a')] = true;
    }

    return result;
}

static std::array<Segments, 10> Analyze(std::string_view signalPatterns)
{
    auto patterns = Split(signalPatterns, ' ');
    std::vector<Segments> segments(patterns.size());
    std::transform(begin(patterns), end(patterns), begin(segments), &ToSegments);
    std::array<Segments, 10> digits;

    for (auto const &s : segments)
    {
        auto const count = s.count();

        if (count == 2)
        {
            digits[1] = s;
        }
        else if (count == 3)
        {
            digits[7] = s;
        }
        else if (count == 4)
        {
            digits[4] = s;
        }
        else if (count == 7)
        {
            digits[8] = s;
        }
    }

    std::erase(segments, digits[1]);
    std::erase(segments, digits[4]);
    std::erase(segments, digits[7]);
    std::erase(segments, digits[8]);

    Segments topLeft;
    Segments middle;

    for (auto const &s : segments)
    {
        auto const count = s.count();

        if (count == 5)
        {
            // 3 doit contenir tous les digits de 1 contrairement a 2 et 5
            if ((s & digits[1]) == digits[1])
            {
                digits[3] = s;

                // trouve le segment de 4 qui n'est pas dans 3
                topLeft = digits[4] & ~digits[3];

                // trouve le segment de 4 qui n'est pas dans 7, moins le segment precedant
                middle = digits[4] & ~digits[7] & ~topLeft;
            }
        }
        else if (count == 6)
        {
            // 6 ne doit pas contenir tous les digits de 7 contrairement a 0 et 9
            if ((s & digits[7]) != digits[7])
            {
                digits[6] = s;
            }
        }
    }

    std::erase(segments, digits[3]);
    std::erase(segments, digits[6]);

    for (auto const &s : segments)
    {
        auto const count = s.count();

        if (count == 5)
        {
            // 5 contiens le segment haut gauche, pas 2
            if ((s & topLeft) == topLeft)
            {
                digits[5] = s;
            }
            else
            {
                digits[2] = s;
            }
        }
        else if (count == 6)
        {
            // 9 contiens le segment du milieu, pas 0
            if ((s & middle) == middle)
            {
                digits[9] = s;
            }
            else
            {
                digits[0] = s;
            }
        }
    }

    return digits;
}

static int MapNumber(auto const &mapping, std::string_view text)
{
    int result = 0;
    int mult = 1000;

    for (auto const &n : Split(text, ' '))
    {
        auto const s = ToSegments(n);
        auto const digit = static_cast<int>(std::find(begin(mapping), end(mapping), s) - begin(mapping));
        result += digit * mult;
        mult /= 10;
    }

    return result;
}

static int Part2(std::string_view entries)
{
    int total = 0;

    for (auto line : Split(entries, '\n'))
    {
        auto const parts = Split(line, '|');
        auto const mapping = Analyze(trim_copy(parts[0]));
        total += MapNumber(mapping, trim_copy(parts[1]));
    }

    return total;
}

int main()
{
    // https://adventofcode.com/2021/day/8
    fmt::print("Day 8, 2021: Seven Segment Search\n");

    Assert(26 == Part1(example::entries));
    Assert(61'229 == Part2(example::entries));

    auto const part1 = Part1(GetInput());
    fmt::print("  Part 1: {}\n", part1);
    Assert(392 == part1);

    auto const part2 = Part2(GetInput());
    fmt::print("  Part 2: {}\n", part2);
    Assert(1'004'688 == part2);
}
