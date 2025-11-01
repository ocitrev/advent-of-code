#include "day15.hpp"

#include "../common.hpp"

#include <initializer_list>
#include <set>

struct Game
{
    using Int = std::uint_fast32_t;
    std::vector<std::pair<Int, Int>> turns;
    std::vector<Int> spoken;
    Int last = 0;
    Int turnNumber = 0;

    Game(std::span<int const> nums)
        : turns(1000)
        , spoken(1000)
    {
        for (int n : nums)
        {
            Speak(static_cast<Int>(n));
        }
    }

    void Speak(Int num)
    {
        auto &p = turns[num];
        p.second = std::exchange(p.first, ++turnNumber);
        ++spoken[num];
        last = num;
    }

    Int Turn()
    {
        Int n = spoken[last] == 1 ? 0 : turnNumber - turns[last].second;
        Speak(n);
        return n;
    }

    Int GetTurn(Int turn)
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
    fmt::print("Day 15, 2020: Rambunctious Recitation\n");

    Assert((1 == Game{std::vector{1, 3, 2}}.GetTurn(2020)));
    Assert((10 == Game{std::vector{2, 1, 3}}.GetTurn(2020)));
    Assert((27 == Game{std::vector{1, 2, 3}}.GetTurn(2020)));
    Assert((78 == Game{std::vector{2, 3, 1}}.GetTurn(2020)));
    Assert((438 == Game{std::vector{3, 2, 1}}.GetTurn(2020)));
    Assert((1836 == Game{std::vector{3, 1, 2}}.GetTurn(2020)));

    auto const part1 = Game{ParseInputNumbers<int, ','>()}.GetTurn(2020);
    fmt::print("  Part 1: {}\n", part1);
    Assert(371 == part1);

    auto const part2 = Game{ParseInputNumbers<int, ','>()}.GetTurn(30'000'000);
    fmt::print("  Part 2: {}\n", part2);
    Assert(352 == part2);
}
