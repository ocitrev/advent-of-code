#pragma once
#include "game_item.hpp"

struct Character
{
    int hp = 100;
    int dammage = 0;
    int armor = 0;
    int mana = 500;
    int spent = 0;

    void Buy(Item const &item)
    {
        armor += item.Armor;
        dammage += item.Dammage;
        spent += item.Cost;
    }

    int Attack(Character &target) const
    {
        int const dmgDone = std::max(1, dammage - target.armor);
        target.hp -= dmgDone;
        return dmgDone;
    }

    [[nodiscard]] bool IsAlive() const
    {
        return hp > 0;
    }

    [[nodiscard]] bool IsDead() const
    {
        return hp <= 0;
    }
};
