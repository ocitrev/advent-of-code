#include "day1.hpp"

#include "../common.hpp"

using namespace std::literals;

static auto Part1(std::string_view lines)
{
    int total = 0;

    for (auto line : Split(lines, '\n'))
    {
        char first = '\0';
        char last = '\0';

        for (char c : line)
        {
            if (isdigit(c))
            {
                if (first == '\0')
                {
                    first = c;
                }

                last = c;
            }
        }

        total += svtoi<int>(std::format("{}{}", first, last));
    }

    return total;
}

static int GetDigit(std::string_view line)
{
    switch (char const c = line.front(); c)
    {
        case '0':
            throw std::runtime_error{"invalid digit"};
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return c - '0';
        case 'o':
            if (line.starts_with("one"sv))
            {
                return 1;
            }
            break;
        case 't':
            if (line.starts_with("two"sv))
            {
                return 2;
            }
            else if (line.starts_with("three"sv))
            {
                return 3;
            }
            break;
        case 'f':
            if (line.starts_with("four"sv))
            {
                return 4;
            }
            else if (line.starts_with("five"sv))
            {
                return 5;
            }
            break;
        case 's':
            if (line.starts_with("six"sv))
            {
                return 6;
            }
            else if (line.starts_with("seven"sv))
            {
                return 7;
            }
            break;
        case 'e':
            if (line.starts_with("eight"sv))
            {
                return 8;
            }
            break;
        case 'n':
            if (line.starts_with("nine"sv))
            {
                return 9;
            }
            break;
    }

    return 0;
}

static auto GetCalibration2(std::string_view line)
{
    trim(line);
    std::vector<int> digits;

    while (true)
    {
        auto const digit = GetDigit(line);
        if (digit != 0)
        {
            digits.push_back(digit);
        }

        line = line.substr(1);

        if (line.empty())
        {
            break;
        }
    }

    return svtoi<int64_t>(std::format("{}{}", digits.front(), digits.back()));
}

static auto Part2(std::string_view lines)
{
    int64_t total = 0;

    for (auto line : Split(lines, '\n'))
    {
        total += GetCalibration2(line);
    }

    return total;
}

int main()
{
    // https://adventofcode.com/2023/day/1
    std::println("Day 1, 2023: Trebuchet?!");

    Assert(142 == Part1(example::part1));
    Assert(281 == Part2(example::part2));
    Assert(29 == GetCalibration2(Split(example::part2, '\n')[0]));
    Assert(83 == GetCalibration2(Split(example::part2, '\n')[1]));
    Assert(13 == GetCalibration2(Split(example::part2, '\n')[2]));
    Assert(24 == GetCalibration2(Split(example::part2, '\n')[3]));
    Assert(42 == GetCalibration2(Split(example::part2, '\n')[4]));
    Assert(14 == GetCalibration2(Split(example::part2, '\n')[5]));
    Assert(76 == GetCalibration2(Split(example::part2, '\n')[6]));

    auto const part1 = Part1(GetInput());
    std::println("  Part 1: {}", part1);
    Assert(54'632 == part1);

    auto const part2 = Part2(GetInput());
    std::println("  Part 2: {}", part2);
    Assert(54'019 == part2);
}
