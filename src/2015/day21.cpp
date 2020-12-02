#include "day21.hpp"
#include "../common/assert.hpp"
#include "game_character.hpp"
#include "game_item.hpp"
#include <fmt/format.h>
#include <vector>

struct Shop
{
    std::vector<Item> weapons{
        {"Dagger", 8, 4, 0},     {"Shortsword", 10, 5, 0}, {"Warhammer", 25, 6, 0},
        {"Longsword", 40, 7, 0}, {"Greataxe", 74, 8, 0},
    };
    std::vector<Item> armors{
        {"Leather", 13, 0, 1},    {"Chainmail", 31, 0, 2},  {"Splintmail", 53, 0, 3},
        {"Bandedmail", 75, 0, 4}, {"Platemail", 102, 0, 5},
    };
    std::vector<Item> rings{
        {"Damage +1", 25, 1, 0},  {"Damage +2", 50, 2, 0},  {"Damage +3", 100, 3, 0},
        {"Defense +1", 20, 0, 1}, {"Defense +2", 40, 0, 2}, {"Defense +3", 80, 0, 3},
    };
};

static Character GetBoss()
{
    return {103, 9, 2};
}

struct Game
{
    Character boss = GetBoss();
    Character player;

    void Buy(Shop const &shop, int w, int a, int r1, int r2)
    {
        player.Buy(shop.weapons[static_cast<std::size_t>(w)]);

        if (a != -1)
            player.Buy(shop.armors[static_cast<std::size_t>(a)]);

        if (r1 != -1)
            player.Buy(shop.rings[static_cast<std::size_t>(r1)]);

        if (r2 != -1)
            player.Buy(shop.rings[static_cast<std::size_t>(r2)]);
    }

    bool Fight()
    {
        while (true)
        {
            player.Attack(boss);

            if (boss.IsDead())
                return true;

            boss.Attack(player);

            if (player.IsDead())
                return false;
        }
    }
};

template <typename PredT>
int RunGame(bool condition, PredT &&pred)
{
    Shop shop;
    std::vector<int> goldSpentPerGame;

    for (int r2 = -1; r2 < static_cast<int>(shop.rings.size()); ++r2)
    {
        for (int r1 = -1; r1 < static_cast<int>(shop.rings.size()); ++r1)
        {
            if (r1 != -1 && r1 <= r2)
                continue;

            for (int a = -1; a < static_cast<int>(shop.armors.size()); ++a)
            {
                for (int w = 0; w < static_cast<int>(shop.weapons.size()); ++w)
                {
                    Game game;
                    game.Buy(shop, w, a, r1, r2);

                    if (condition == game.Fight())
                    {
                        goldSpentPerGame.push_back(game.player.spent);
                    }
                }
            }
        }
    }

    return *std::min_element(begin(goldSpentPerGame), end(goldSpentPerGame), std::forward<PredT>(pred));
}

int main()
{
    // https://adventofcode.com/2015/day/21
    fmt::print("Day 21: RPG Simulator 20XX\n");

    auto const part1 = RunGame(true, std::less<int>{});
    fmt::print("  Part 1: {}\n", part1);
    Assert(121 == part1);

    auto const part2 = RunGame(false, std::greater<int>{});
    fmt::print("  Part 2: {}\n", part2);
    Assert(201 == part2);
}
