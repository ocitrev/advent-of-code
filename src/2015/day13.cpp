#include "day13.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include "../common/utils.hpp"
#include <fmt/format.h>
#include <string>
#include <vector>

struct Relation
{
    std::string left;
    std::string right;
    int happiness;

    Relation(std::string_view left_, std::string_view right_, int happiness_)
        : left(left_)
        , right(right_)
        , happiness(happiness_)
    {
        rtrim_if(right,
            [](int ch)
            {
                return ch == '.';
            });
    }
};

static int GetHappiness(std::vector<Relation> const &relations, std::string_view left, std::string_view right)
{
    int totalHappiness = 0;
    auto iter = std::find_if(begin(relations), end(relations),
        [&](Relation const &rel)
        {
            return rel.left == left && rel.right == right;
        });

    if (iter != end(relations))
        totalHappiness += iter->happiness;

    iter = std::find_if(begin(relations), end(relations),
        [&](Relation const &rel)
        {
            return rel.left == right && rel.right == left;
        });

    if (iter != end(relations))
        totalHappiness += iter->happiness;

    return totalHappiness;
}

static std::vector<Relation> ParseRelations(std::string_view text)
{
    std::vector<Relation> relations;

    for (auto &&line : Split(text, '\n'))
    {
        auto const parts = Split(line, ' ');
        Relation &r = relations.emplace_back(parts[0], parts[10], svtoi(parts[3]));

        if (parts[2] == "lose")
            r.happiness = -r.happiness;
    }

    return relations;
}

static std::vector<std::string> GetPeople(std::vector<Relation> const &relations)
{
    std::vector<std::string> people;

    for (auto &&r : relations)
    {
        insert_sorted(people, r.left);
    }

    return people;
}

static int ComputeBestHappiness(std::vector<Relation> const &relations, std::vector<std::string> people)
{
    int bestHappiness = 0;

    do
    {
        std::string prev;
        int totalHappiness = 0;

        for (auto &&person : people)
        {
            if (not prev.empty())
            {
                totalHappiness += GetHappiness(relations, prev, person);
            }

#if DEBUG_PRINT
            fmt::print("{} -> ", person);
#endif
            prev = person;
        }

        totalHappiness += GetHappiness(relations, prev, people.front());
        bestHappiness = std::max(bestHappiness, totalHappiness);
#if DEBUG_PRINT
        fmt::print("{}\n", totalHappiness);
#endif
    } while (next_necklace(begin(people), end(people)));

    return bestHappiness;
}

static int ComputeBestHappiness(std::string_view text)
{
    std::vector<Relation> relations = ParseRelations(text);
    std::vector<std::string> people = GetPeople(relations);
    return ComputeBestHappiness(relations, people);
}

static int Part1()
{
    return ComputeBestHappiness(input::data);
}

static int Part2()
{
    std::vector<Relation> relations = ParseRelations(input::data);
    std::vector<std::string> people = GetPeople(relations);
    using namespace std::string_literals;
    insert_sorted(people, "Moi"s);
    return ComputeBestHappiness(relations, people);
}

int main()
{
    // https://adventofcode.com/2015/day/13
    fmt::print("Day 13, 2015: Knights of the Dinner Table\n");

    Assert(330 == ComputeBestHappiness(example::data));

    int const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(709 == part1);

    int const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(668 == part2);
}
