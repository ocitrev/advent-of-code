#include "day11.hpp"
#include "../common.hpp"
#include <ranges>

static std::vector<int> ParseItems(std::string_view line)
{
    std::vector<int> items;

    for (auto startItem : Split(Split(line, ':')[1], ','))
    {
        trim(startItem);
        items.push_back(svtoi(startItem));
    }

    return items;
}

std::function<int(int)> ParseOperation(std::string_view line)
{
    auto opParts = Split(trim_copy(Split(line, '=')[1]), ' ');

    if (opParts[1] == "*")
    {
        if (opParts[2] == "old")
        {
            return [](int v)
            {
                return v * v;
            };
        }

        int operand = svtoi(opParts[2]);
        return [operand](int v)
        {
            return v * operand;
        };
    }
    else if (opParts[1] == "+")
    {
        if (opParts[2] == "old")
        {
            return [](int v)
            {
                return v + v;
            };
        }

        int operand = svtoi(opParts[2]);
        return [operand](int v)
        {
            return v + operand;
        };
    }

    throw std::invalid_argument("line");
}

static int ParseTest(std::string_view line)
{
    return svtoi(Split(line, ' ').back());
}

static std::pair<int, int> ParseRules(std::string_view line1, std::string_view line2)
{
    trim(line1);
    trim(line2);
    int const throwTrue = svtoi(Split(line1, ' ').back());
    int const throwFalse = svtoi(Split(line2, ' ').back());
    return std::make_pair(throwFalse, throwTrue);
}

using Item = int;

struct Monkey
{
    int id = 0;
    std::vector<Item> items;
    std::function<Item(Item)> operation;
    int divisble = 0;
    std::pair<int, int> rules;
    int nbInspections = 0;

    static Monkey Parse(std::string_view block)
    {
        auto lines = Split(block, '\n');
        Monkey m;
        m.id = svtoi(Split(lines[0], ' ')[1]);
        m.items = ParseItems(lines[1]);
        m.operation = ParseOperation(lines[2]);
        m.divisble = ParseTest(lines[3]);
        m.rules = ParseRules(lines[4], lines[5]);
        return m;
    }

    Item Inspect(Item item)
    {
        ++nbInspections;
        return operation(item) / 3;
    }
};

struct Game
{
    std::vector<Monkey> monkeys;

    void ParseNotes(std::string_view notes)
    {
        for (auto block : Split(notes, "\n\n"))
        {
            monkeys.push_back(Monkey::Parse(block));
        }
    }

    void Round()
    {
        for (Monkey &monkey : monkeys)
        {
            Turn(monkey);
        }
    }

    void Turn(Monkey &monkey)
    {
        auto items = std::exchange(monkey.items, {});

        for (int &item : items)
        {
            item = monkey.Inspect(item);
            auto const monkeyIndex
                = static_cast<size_t>(item % monkey.divisble == 0 ? monkey.rules.second : monkey.rules.first);
            monkeys.at(monkeyIndex).items.push_back(item);
        }
    }
};

static int PlayGame(std::string_view notes, int rounds)
{
    Game g;
    g.ParseNotes(notes);

    for (int i = 0; i != rounds; ++i)
    {
        g.Round();
    }

    std::vector<int> inspections(g.monkeys.size());
    std::transform(begin(g.monkeys), end(g.monkeys), begin(inspections),
        [](Monkey const &monkey)
        {
            return monkey.nbInspections;
        });
    std::sort(begin(inspections), end(inspections), std::greater{});
    return inspections[0] * inspections[1];
}

static int Part1()
{
    return PlayGame(input::notes, 20);
}

static int Part2()
{
    return 0;
}

int main()
{
    // https://adventofcode.com/2022/day/11
    fmt::print("Day 11, 2022: Monkey in the Middle\n");

    Assert(10605 == PlayGame(example::notes, 20));

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(50830 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    // Assert( == part2);
}
