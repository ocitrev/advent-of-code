#include "day15.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include "../common/utils.hpp"
#include <array>
#include <fmt/format.h>
#include <fstream>
#include <numeric>
#include <utility>
#include <vector>

struct Ingredient
{
    std::string name;
    int capacity = 0;
    int durability = 0;
    int flavor = 0;
    int texture = 0;
    int calories = 0;

    Ingredient operator*(int nbSpoon) const
    {
        return {
            name, nbSpoon * capacity, nbSpoon * durability, nbSpoon * flavor, nbSpoon * texture, nbSpoon * calories};
    }

    Ingredient &operator+=(Ingredient const &other)
    {
        capacity += other.capacity;
        durability += other.durability;
        flavor += other.flavor;
        texture += other.texture;
        calories += other.calories;
        return *this;
    }

    [[nodiscard]] int Score() const
    {
        return std::max(0, capacity) * std::max(0, durability) * std::max(0, flavor) * std::max(0, texture);
        // return capacity * durability * flavor * texture;
    }
};

[[nodiscard]] static std::vector<Ingredient> PraseIngredients(std::string_view text)
{
    std::vector<Ingredient> ingredients;

    for (auto &&line : Split(text, '\n'))
    {
        auto const parts = Split(line, ' ');

        constexpr size_t nameIndex = 0;
        constexpr size_t capacityIndex = 2;
        constexpr size_t durabilityIndex = 4;
        constexpr size_t flavorIndex = 6;
        constexpr size_t textureIndex = 8;
        constexpr size_t caloriesIndex = 10;

        auto &ingredient = ingredients.emplace_back(Ingredient{
            std::string{parts[nameIndex]},
            svtoi(parts[capacityIndex]),
            svtoi(parts[durabilityIndex]),
            svtoi(parts[flavorIndex]),
            svtoi(parts[textureIndex]),
            svtoi(parts[caloriesIndex]),
        });

        rtrim_if(ingredient.name,
                 [](int ch)
                 {
                     return std::ispunct(ch);
                 });
    }

    return ingredients;
}

template <typename IterT>
[[nodiscard]] static Ingredient CombineIngredients(IterT first, IterT last)
{
    Ingredient total;

    for (auto iter = first; iter != last; ++iter)
    {
        total += iter->first * iter->second;
    }

    return total;
}

template <typename ContainerT>
[[nodiscard]] static Ingredient CombineIngredients(ContainerT const &cont)
{
    return CombineIngredients(begin(cont), end(cont));
}

static void Tests()
{
#ifdef NDEBUG
    auto const ingredients = PraseIngredients(example::text);

    {
        std::array values{std::make_pair(ingredients[0], 44), std::make_pair(ingredients[1], 56)};
        auto total = CombineIngredients(values);
        Assert(62842880 == total.Score());
    }

    {
        std::array values{std::make_pair(ingredients[0], 40), std::make_pair(ingredients[1], 60)};
        auto total = CombineIngredients(values);
        Assert(500 == total.calories);
        Assert(57600000 == total.Score());
    }
#endif
}

static std::pair<int, int> Run()
{
    auto const ingredients = PraseIngredients(input::text);
    int maxScore = 0;
    int maxScore500 = 0;

    for (int a = 1; a <= 100; ++a)
    {
        for (int b = 1; b <= 100 - a; ++b)
        {
            for (int c = 1; c <= 100 - a - b; ++c)
            {
                int const d = 100 - a - b - c;

                if (d == 0)
                    continue;

                std::array biscuit{
                    std::make_pair(ingredients[0], a),
                    std::make_pair(ingredients[1], b),
                    std::make_pair(ingredients[2], c),
                    std::make_pair(ingredients[3], d),
                };

                auto const total = CombineIngredients(biscuit);
                auto const score = total.Score();

                if (total.calories == 500 && score > maxScore500)
                    maxScore500 = score;

                if (score > maxScore)
                    maxScore = score;
            }
        }
    }

    return std::make_pair(maxScore, maxScore500);
}

int main()
{
    // https://adventofcode.com/2015/day/15
    fmt::print("Day 15, 2015: Science for Hungry People\n");

    Tests();

    auto const [part1, part2] = Run();
    fmt::print("  Part 1: {}\n", part1);
    Assert(21367368 == part1);
    fmt::print("  Part 2: {}\n", part2);
    Assert(1766400 == part2);
}
