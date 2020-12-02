#include "day22.hpp"
#include "../common/assert.hpp"
#include "game_character.hpp"
#include "game_spells.hpp"
#include <fmt/format.h>
#include <map>

static Character GetBoss()
{
    return {71, 10};
}

struct Game
{
    Character boss = GetBoss();
    Character wizard{50};
    std::map<std::string, Effect> effects;
    std::vector<Spell> const spells{
        Spell{"Recharge", 229, Effect{5, 0, 101, 0}},
        Spell{"Shield", 113, Effect{6, 0, 0, 7}},
        Spell{"Poison", 173, Effect{6, 3, 0, 0}},
        Spell{"Drain", 73, 2, 2},
        Spell{"Magic Missile", 53, 4},
    };

    void ApplyEffects()
    {
        wizard.armor = 0;

        for (auto &[name, eff] : effects)
        {
            boss.hp -= eff.dammage;
            wizard.mana += eff.mana;
            wizard.armor += eff.armor;
            --eff.turns;

            if (eff.mana > 0)
                fmt::print("{} provides {} mana; its timer is now {}.\n", name, eff.mana, eff.turns);

            if (eff.armor > 0)
                fmt::print("{}'s timer is now {}.\n", name, eff.turns);

            if (eff.dammage > 0)
                fmt::print("{} deals {} dammage; its timer is now {}.\n", name, eff.dammage, eff.turns);
        }

        for (auto iter = begin(effects); iter != end(effects);)
        {
            if (iter->second.turns == 0)
            {
                fmt::print("{} wears off.\n", iter->first);
                iter = effects.erase(iter);
            }
            else
            {
                ++iter;
            }
        }
    }

    bool CastSpell(Spell const &spell)
    {
        if (wizard.mana < spell.cost)
        {
            // not enough mana
            return false;
        }

        if (spell.effect.has_value())
        {
            if (auto effectIt = effects.find(spell.name); effectIt != end(effects) && effectIt->second.turns > 0)
            {
                // same spell already active
                return false;
            }

            effects.insert(std::make_pair(spell.name, *spell.effect));
        }

        if (spell.dammage > 0 && spell.heal > 0)
            fmt::print("Player casts {}, dealing {} dammage and healing {} hit points.\n", spell.name, spell.dammage,
                       spell.heal);
        else if (spell.dammage > 0)
            fmt::print("Player casts {}, dealing {} dammage.\n", spell.name, spell.dammage);
        else
            fmt::print("Player casts {}.\n", spell.name);

        boss.hp -= spell.dammage;
        wizard.mana -= spell.cost;
        wizard.hp += spell.heal;
        return true;
    }

    void TurnStart(std::string_view turn)
    {
        fmt::print("\n-- {} turn --\n- Player has {} hit points, {} armor, {} mana\n- Boss has {} hit points\n", turn,
                   wizard.hp, wizard.armor, wizard.mana, boss.hp);

        ApplyEffects();
    }

    void WizardTurn()
    {
        for (auto const &s : spells)
        {
            if (CastSpell(s))
                break;
        }
    }

    void BossTurn()
    {
        int const dammage = boss.Attack(wizard);
        fmt::print("Boss attacks for {} - {} = {} damage,\n", boss.dammage, wizard.armor, dammage);
    }

    bool Run()
    {
        while (true)
        {
            TurnStart("Player");
            if (boss.IsDead())
                return true;
            WizardTurn();
            if (boss.IsDead())
                return true;

            TurnStart("Boss");
            if (boss.IsDead())
                return true;
            BossTurn();
            if (wizard.IsDead())
                return false;
        }
    }
};

int main()
{
    // https://adventofcode.com/2015/day/22
    fmt::print("Day 22: Wizard Simulator 20XX\n");

    Game g;
    g.Run();
}
