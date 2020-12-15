#include "day15.hpp"
#include "../common/assert.hpp"
#include <fmt/format.h>
#include <initializer_list>
#include <set>
#include <vector>

struct Game
{
    std::vector<std::pair<unsigned int, unsigned int>> turns;
    std::vector<unsigned int> spoken;
    unsigned int last = 0;
    unsigned int turnNumber = 0;

    Game(std::initializer_list<int> nums)
        : turns(1000)
        , spoken(1000)
    {
        for (int n : nums)
        {
            Speak(static_cast<unsigned int>(n));
        }
    }

    void Speak(unsigned int num)
    {
        auto &p = turns[num];
        p.second = std::exchange(p.first, ++turnNumber);
        ++spoken[num];
        last = num;
    }

    unsigned int Turn()
    {
        unsigned int n = spoken[last] == 1 ? 0 : turnNumber - turns[last].second;
        Speak(n);
        return n;
    }

    unsigned int GetTurn(unsigned int turn)
    {
        turns.resize(turn);
        spoken.resize(turn);

        while (turnNumber < turn - 1)
        {
            Turn();
        }

        return Turn();
    }
};

int main()
{
    // https://adventofcode.com/2020/day/15
    fmt::print("Day 15: Rambunctious Recitation\n");

    Assert((1 == Game{1, 3, 2}.GetTurn(2020)));
    Assert((10 == Game{2, 1, 3}.GetTurn(2020)));
    Assert((27 == Game{1, 2, 3}.GetTurn(2020)));
    Assert((78 == Game{2, 3, 1}.GetTurn(2020)));
    Assert((438 == Game{3, 2, 1}.GetTurn(2020)));
    Assert((1836 == Game{3, 1, 2}.GetTurn(2020)));

    auto const part1 = Game{input::numbers}.GetTurn(2020);
    fmt::print("  Part 1: {}\n", part1);
    Assert(371 == part1);

    auto const part2 = Game{input::numbers}.GetTurn(30'000'000);
    fmt::print("  Part 2: {}\n", part2);
    Assert(352 == part2);
}
