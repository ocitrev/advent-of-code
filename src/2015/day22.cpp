#include "day22.hpp"

#include "game_character.hpp"

#include "../common/assert.hpp"
#include "../common/string.hpp"

#include <map>
#include <optional>
#include <print>
#include <vector>

static int GetIntAfter(std::string_view text, std::string_view search)
{
    if (auto const pos = text.find(search); pos != text.npos)
    {
        return svtoi(text.substr(pos + search.size()));
    }

    return 0;
}

static Character GetBoss()
{
    auto const input = GetInput();
    auto const hp = GetIntAfter(input, "Hit Points: ");
    auto const damage = GetIntAfter(input, "Damage: ");
    return {hp, damage};
}

struct Game
{
    Character boss = GetBoss();
    Character wizard{50};
    int rechargeTimer = 0;
    int shieldTimer = 0;
    int poisonTimer = 0;
    bool hard = false;

    enum SpellId
    {
        Recharge,
        Shield,
        Poison,
        Drain,
        MagicMissile,
    };

    struct Spell
    {
        SpellId name;
        int cost = 0;
        int dammage = 0;
        int heal = 0;
        bool effect = false;
    };

    inline static std::vector<Spell> const spells{
        Spell{SpellId::Recharge, 229, 0, 0, true},
        Spell{SpellId::Shield, 113, 0, 0, true},
        Spell{SpellId::Poison, 173, 0, 0, true},
        Spell{SpellId::Drain, 73, 2, 2, false},
        Spell{SpellId::MagicMissile, 53, 4, 0, false},
    };

    void ApplyEffects()
    {
        wizard.armor = shieldTimer != 0 ? 7 : 0;

        if (rechargeTimer > 0)
        {
            wizard.mana += 101;
        }

        if (poisonTimer > 0)
        {
            boss.hp -= 3;
        }

        rechargeTimer = std::max(0, rechargeTimer - 1);
        shieldTimer = std::max(0, shieldTimer - 1);
        poisonTimer = std::max(0, poisonTimer - 1);
    }

    [[nodiscard]] bool CanCast(Spell const &spell) const
    {
        // check if enough mana
        if (wizard.mana < spell.cost)
        {
            return false;
        }

        if (spell.name == SpellId::Recharge)
        {
            return rechargeTimer == 0;
        }

        if (spell.name == SpellId::Shield)
        {
            return shieldTimer == 0;
        }

        if (spell.name == SpellId::Poison)
        {
            return poisonTimer == 0;
        }

        return true;
    }

    bool CastSpell(Spell const &spell)
    {
        if (not CanCast(spell))
        {
            return false;
        }

        if (spell.name == SpellId::Recharge)
        {
            rechargeTimer = 5;
        }

        if (spell.name == SpellId::Shield)
        {
            shieldTimer = 6;
        }

        if (spell.name == SpellId::Poison)
        {
            poisonTimer = 6;
        }

        boss.hp -= spell.dammage;
        wizard.mana -= spell.cost;
        wizard.hp += spell.heal;
        return true;
    }

    int WizardTurn()
    {
        if (hard)
        {
            --wizard.hp;

            if (wizard.IsDead())
            {
                return 1 << 30;
            }
        }

        ApplyEffects();

        if (boss.IsDead())
        {
            return 0;
        }

        int best = 1 << 30;

        for (auto const &s : spells)
        {
            if (s.cost > best)
            {
                continue;
            }

            if (CanCast(s))
            {
                Game subGame = *this;
                subGame.CastSpell(s);
                best = std::min(best, s.cost + subGame.Play(false));
            }
        }

        return best;
    }

    int BossTurn()
    {
        ApplyEffects();

        if (boss.IsDead())
        {
            return 0;
        }

        boss.Attack(wizard);

        if (wizard.IsDead())
        {
            return 1 << 30;
        }

        return Play(true);
    }

    int Play(bool playerTurn)
    {
        if (playerTurn)
        {
            return WizardTurn();
        }

        return BossTurn();
    }
};

static void Example()
{
#ifndef NDEBUG
    {
        Game g1;
        g1.boss = Character{13, 8};
        g1.wizard = Character{10, 250};
        Assert(226 == g1.Play(true));

        Game g2;
        g2.hard = true;
        g2.boss = Character{13, 8};
        g2.wizard = Character{10, 250};
        Assert(339 == g2.Play(true));
    }

    {
        Game g1;
        g1.boss = Character{14, 8};
        g1.wizard = Character{10, 250};
        Assert(339 == g1.Play(true));

        Game g2;
        g2.hard = true;
        g2.boss = Character{14, 8};
        g2.wizard = Character{10, 250};
        Assert(339 == g2.Play(true));
    }
#endif
}

static int Part1()
{
    Game g;
    return g.Play(true);
}

static int Part2()
{
    Game g;
    g.hard = true;
    return g.Play(true);
}

int main()
{
    // https://adventofcode.com/2015/day/22
    std::print("Day 22, 2015: Wizard Simulator 20XX\n");

    Example();

    auto const part1 = Part1();
    std::print("  Part 1: {}\n", part1);
    Assert(1824 == part1);

    auto const part2 = Part2();
    std::print("  Part 2: {}\n", part2);
    Assert(1937 == part2);
}
