#include "day19.hpp"

#include "../common/assert.hpp"
#include "../common/string.hpp"

#include <algorithm>
#include <functional>
#include <map>
#include <print>
#include <random>
#include <set>

using Grammar = std::multimap<std::string, std::string>;

int CountMolecules(Grammar const &grammar, std::string_view input)
{
    std::set<std::string> molecules;

    for (auto const &[from, to] : grammar)
    {
        auto pos = input.find(from);

        while (pos != std::string::npos)
        {
            std::string inputText{input};
            inputText.replace(pos, from.size(), to, 0, to.size());
            molecules.insert(std::move(inputText));
            pos = input.find(from, pos + from.size());
        }
    }

    return static_cast<int>(molecules.size());
}

Grammar ParseGrammar(std::string_view text)
{
    Grammar t;
    for (auto &&line : Split(text, '\n'))
    {
        auto pos = line.find(" => ");
        t.insert(std::make_pair(line.substr(0, pos), line.substr(pos + 4)));
    }

    return t;
}

static auto &GetRNG()
{
    static std::mt19937 rng(std::random_device{}());
    return rng;
}

int GetNumberOfSteps(Grammar const &grammar, std::string_view input)
{
    int count = 0;
    std::string molecule{input};
    std::map<std::string, std::string> reverseMap;
    std::vector<std::string> tos;
    tos.reserve(grammar.size());

    for (auto const &[from, to] : grammar)
    {
        reverseMap.try_emplace(to, from);
        tos.push_back(to);
    }

    // du plus grand au plus petit, pour favoriser un algo "greedy"
    std::sort(begin(tos), end(tos),
        [](std::string const &a, std::string const &b)
        {
            return std::make_pair(a.size(), a) > std::make_pair(b.size(), b);
        });

    while (molecule != "e")
    {
        std::string const old = molecule;

        for (auto const &to : tos)
        {
            auto pos = molecule.find(to);

            while (pos != std::string::npos)
            {
                auto const &from = reverseMap.find(to)->second;
                molecule.replace(pos, to.size(), from, 0, from.size());
                ++count;
                pos = molecule.find(to, pos + from.size());
            }
        }

        if (old == molecule)
        {
            count = 0;
            molecule = input;
            // si on ne fait pas de progres, randomize l'input
            // TODO: Implementer l'algo CYK pour eviter de se fier au random
            // https://en.wikipedia.org/wiki/CYK_algorithm
            std::shuffle(begin(tos), end(tos), GetRNG());
        }
    }

    return count;
}

static auto ParseInput()
{
    auto parts = Split(GetInput(), "\n\n");
    return std::make_pair(parts[0], parts[1]);
}

int main()
{
    // https://adventofcode.com/2015/day/19
    std::println("Day 19, 2015: Medicine for Rudolph");

    Assert(4 == CountMolecules(ParseGrammar(example::grammar1), example::molecule1));
    Assert(7 == CountMolecules(ParseGrammar(example::grammar1), example::molecule2));
    Assert(3 == GetNumberOfSteps(ParseGrammar(example::grammar2), example::molecule1));
    Assert(6 == GetNumberOfSteps(ParseGrammar(example::grammar2), example::molecule2));

    auto const [grammar, molecule] = ParseInput();

    auto const part1 = CountMolecules(ParseGrammar(grammar), molecule);
    std::println("  Part 1: {}", part1);
    Assert(535 == part1);

    auto const part2 = GetNumberOfSteps(ParseGrammar(grammar), molecule);
    std::println("  Part 2: {}", part2);
    Assert(212 == part2);
}
