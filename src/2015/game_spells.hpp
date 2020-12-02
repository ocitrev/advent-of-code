#pragma once
#include <optional>
#include <string>
#include <vector>

struct Effect
{
    int turns = 0;
    int dammage = 0;
    int mana = 0;
    int armor = 0;
};

struct Spell
{
    std::string name;
    int cost = 0;
    int dammage = 0;
    int heal = 0;
    std::optional<Effect> effect;

    Spell(std::string name, int cost, int dammage)
        : name{std::move(name)}
        , cost(cost)
        , dammage(dammage)
    {
    }

    Spell(std::string name, int cost, int dammage, int heal)
        : name{std::move(name)}
        , cost(cost)
        , dammage(dammage)
        , heal(heal)
    {
    }

    Spell(std::string name, int cost, Effect effect)
        : name{std::move(name)}
        , cost(cost)
        , effect{std::forward<Effect>(effect)}
    {
    }
};
