#include "day4.hpp"
#include "../common.hpp"

struct Card
{
    int id = 0;
    int instances = 1;
    int matchCount = 0;

    Card(std::string_view text)
    {
        auto const idAndNumbers = Split(text, ':');
        id = svtoi(Split(idAndNumbers[0], ' ')[1]);

        auto const winnerAndMine = Split(idAndNumbers[1], '|');

        std::unordered_set<int> winners;
        for (auto numberText : Split(trim_copy(winnerAndMine[0]), ' '))
        {
            if (numberText.empty())
            {
                continue;
            }

            winners.insert(svtoi(numberText));
        }

        for (auto numberText : Split(trim_copy(winnerAndMine[1]), ' '))
        {
            if (numberText.empty())
            {
                continue;
            }

            if (winners.contains(svtoi(numberText)))
            {
                ++matchCount;
            }
        }
    }

    [[nodiscard]] int GetScore() const
    {
        if (matchCount == 0)
            return 0;

        return static_cast<int>(std::pow(2, matchCount - 1));
    }

    void Duplicate(int times)
    {
        instances += times;
    }
};

static std::vector<Card> ParseCards(std::string_view cards)
{
    std::vector<Card> result;

    for (auto line : Split(cards))
    {
        result.emplace_back(line);
    }

    return result;
}

static auto GetTotal(std::string_view cards)
{
    int total = 0;

    for (auto const &card : ParseCards(cards))
    {
        total += card.GetScore();
    }

    return total;
}

static auto Part1()
{
    return GetTotal(GetInput());
}

int CountWinningCards(std::string_view cardsLines)
{
    auto cards = ParseCards(cardsLines);
    auto const last = end(cards);

    for (auto iter = begin(cards); iter != last; ++iter)
    {
        auto &card = *iter;
        auto winEnd = std::next(iter, card.matchCount + 1);

        for (auto winIter = std::next(iter); winIter != winEnd; ++winIter)
        {
            winIter->Duplicate(card.instances);
        }
    }

    return std::transform_reduce(begin(cards), end(cards), 0, std::plus{},
        [](Card const &card)
        {
            return card.instances;
        });
}

static auto Part2()
{
    return CountWinningCards(GetInput());
}

int main()
{
    // https://adventofcode.com/2023/day/4
    fmt::print("Day 4, 2023: Scratchcards\n");

    Assert(13 == GetTotal(example::cards));
    Assert(30 == CountWinningCards(example::cards));

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(24848 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(7258152 == part2);
}
