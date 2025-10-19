#include "day11.hpp"
#include "../common.hpp"
#include <gsl/gsl>
#include <ranges>

using Item = std::int64_t;

static std::vector<Item> ParseItems(std::string_view line)
{
    std::vector<Item> items;

    for (auto startItem : Split(Split(line, ':')[1], ','))
    {
        trim(startItem);
        items.push_back(svtoi<Item>(startItem));
    }

    return items;
}

std::function<Item(Item)> ParseOperation(std::string_view line)
{
    auto opParts = Split(trim_copy(Split(line, '=')[1]), ' ');

    if (opParts[1] == "*")
    {
        if (opParts[2] == "old")
        {
            return [](Item v)
            {
                return v * v;
            };
        }

        Item const operand = svtoi<Item>(opParts[2]);
        return [operand](Item v)
        {
            return v * operand;
        };
    }
    else if (opParts[1] == "+")
    {
        if (opParts[2] == "old")
        {
            return [](Item v)
            {
                return v + v;
            };
        }

        Item const operand = svtoi<Item>(opParts[2]);
        return [operand](Item v)
        {
            return v + operand;
        };
    }

    throw std::invalid_argument("line");
}

static Item ParseTest(std::string_view line)
{
    return svtoi<Item>(Split(line, ' ').back());
}

static std::pair<int, int> ParseRules(std::string_view line1, std::string_view line2)
{
    trim(line1);
    trim(line2);
    int const throwTrue = svtoi(Split(line1, ' ').back());
    int const throwFalse = svtoi(Split(line2, ' ').back());
    return std::make_pair(throwFalse, throwTrue);
}

struct Monkey
{
    int id = 0;
    std::vector<Item> items;
    std::function<Item(Item)> operation;
    Item divisble = 0;
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
        return operation(item);
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

    template <class WorryManager>
    void Round(WorryManager &&wm)
    {
        for (Monkey &monkey : monkeys)
        {
            Turn(monkey, wm);
        }
    }

    template <class WorryManager>
    void Turn(Monkey &monkey, WorryManager &&wm)
    {
        auto items = std::exchange(monkey.items, {});

        for (auto &item : items)
        {
            item = wm(monkey.Inspect(item));
            auto const monkeyIndex
                = static_cast<size_t>(item % monkey.divisble == 0 ? monkey.rules.second : monkey.rules.first);
            monkeys.at(monkeyIndex).items.push_back(item);
        }
    }
};

static std::uint64_t GetScore(Game const &game)
{
    Expects(game.monkeys.size() >= 2);

    std::vector<std::uint64_t> inspections(game.monkeys.size());
    std::transform(begin(game.monkeys), end(game.monkeys), begin(inspections),
        [](Monkey const &monkey)
        {
            return monkey.nbInspections;
        });

    std::sort(begin(inspections), end(inspections), std::greater{});
    return inspections[0] * inspections[1];
}

static auto PlayGameDivideBy3(std::string_view notes, int rounds)
{
    Game game;
    game.ParseNotes(notes);

    auto divideBy3 = [](Item value)
    {
        return value / Item{3};
    };

    for (int i = 0; i != rounds; ++i)
    {
        game.Round(divideBy3);
    }

    return GetScore(game);
}

static auto PlayGameModulo(std::string_view notes, int rounds)
{
    Game game;
    game.ParseNotes(notes);

    Item const common = std::transform_reduce(begin(game.monkeys), end(game.monkeys), Item{1}, &std::lcm<Item, Item>,
        [](Monkey const &monkey)
        {
            return monkey.divisble;
        });

    auto wm = [common](Item value)
    {
        return value % common;
    };

    for (int i = 0; i != rounds; ++i)
    {
        game.Round(wm);
    }

    return GetScore(game);
}

static auto Part1()
{
    return PlayGameDivideBy3(GetInput(), 20);
}

static auto Part2()
{
    return PlayGameModulo(GetInput(), 10'000);
}

int main()
{
    // https://adventofcode.com/2022/day/11
    fmt::print("Day 11, 2022: Monkey in the Middle\n");

    Assert(10'605 == PlayGameDivideBy3(example::notes, 20));
    Assert(6 * 4 == PlayGameModulo(example::notes, 1));
    Assert(103 * 99 == PlayGameModulo(example::notes, 20));
    Assert(5'204 * 5'192 == PlayGameModulo(example::notes, 1'000));
    Assert(2'713'310'158 == PlayGameModulo(example::notes, 10'000));

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(50'830 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(14'399'640'002 == part2);
}
