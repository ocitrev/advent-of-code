#include "day5.hpp"

#include "../cpp-utils.hpp"

static void ParseCrates(std::string_view line, std::vector<std::vector<char>> &stacks)
{
    size_t index = 0;
    size_t nbCrates = 0;

    for (char c : line)
    {
        if ((index + 3) % 4 != 0)
        {
            ++index;
            continue;
        }

        auto const crateIndex = ((index + 3) / 4) - 1;

        if (c >= 'A' && c <= 'Z')
        {
            if (crateIndex >= stacks.size())
            {
                stacks.resize(crateIndex + 1);
            }

            stacks[crateIndex].push_back(c);
        }
        else if (c >= '0' && c <= '9')
        {
            nbCrates = std::max(nbCrates, static_cast<size_t>(c - '0'));
        }

        ++index;
    }

    if (nbCrates > stacks.size())
    {
        stacks.resize(nbCrates);
    }
}

enum struct Crane
{
    Mover9000 = 9000,
    Mover9001 = 9001,
};

template <Crane crane>
static void ParseProcedure(std::string_view text, std::vector<std::vector<char>> &stacks)
{
    auto const parts = Split(text, ' ');

    int const nb = svtoi(parts[1]);
    auto &from = stacks[svtoi<size_t>(parts[3]) - 1];
    auto &to = stacks[svtoi<size_t>(parts[5]) - 1];

    if constexpr (crane == Crane::Mover9001)
    {
        auto first = from.end() - nb;
        auto last = from.end();
        to.insert(end(to), first, last);
        from.erase(first, last);
    }
    else
    {
        for (int i = 0; i != nb; ++i)
        {
            to.push_back(from.back());
            from.pop_back();
        }
    }
}

static std::string GetTopOfAllStacks(std::vector<std::vector<char>> const &stacks)
{
    std::string top(stacks.size(), '\0');
    std::transform(begin(stacks), end(stacks), begin(top),
        [](auto const &stack)
        {
            return stack.back();
        });
    return top;
}

template <Crane crane>
static std::string ProcessStacks(std::string_view text)
{
    std::vector<std::vector<char>> stacks;
    bool parseCrates = true;

    for (auto line : Split(text, '\n'))
    {
        if (parseCrates)
        {
            if (line.empty())
            {
                for (auto &s : stacks)
                {
                    std::reverse(begin(s), end(s));
                }

                parseCrates = false;
                continue;
            }

            ParseCrates(line, stacks);
        }
        else
        {
            ParseProcedure<crane>(line, stacks);
        }
    }

    return GetTopOfAllStacks(stacks);
}

static std::string Part1()
{
    return ProcessStacks<Crane::Mover9000>(GetInput());
}

static std::string Part2()
{
    return ProcessStacks<Crane::Mover9001>(GetInput());
}

int main()
{
    // https://adventofcode.com/2022/day/5
    std::println("Day 5, 2022: Supply Stacks");

    Assert("CMZ" == ProcessStacks<Crane::Mover9000>(example::text));
    Assert("MCD" == ProcessStacks<Crane::Mover9001>(example::text));

    auto const part1 = Part1();
    std::println("  Part 1: {}", part1);
    Assert("SHMSDGZVC" == part1);

    auto const part2 = Part2();
    std::println("  Part 2: {}", part2);
    Assert("VRZGHDFBQ" == part2);
}
