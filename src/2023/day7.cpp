#include "day7.hpp"
#include "../common.hpp"
#include <set>

using namespace std::literals;

enum struct HandType
{
    HighCard,
    OnePair,
    TwoPair,
    ThreeOfAKind,
    FullHouse,
    FourOfAKind,
    FiveOfAKind,
};

static constexpr int CardToValue(char c, bool joker)
{
    switch (c)
    {
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
        return 4;
    case '5':
        return 5;
    case '6':
        return 6;
    case '7':
        return 7;
    case '8':
        return 8;
    case '9':
        return 9;
    case 'T':
        return 10;
    case 'J':
        return joker ? 1 : 11;
    case 'Q':
        return 12;
    case 'K':
        return 13;
    case 'A':
        return 14;
    }

    return 0;
}

struct Hand;
static std::pair<bool, int> CountSameCards(Hand const &hand, int nb, int exclude = 0);
static HandType GetHandType(Hand const &hand);
static HandType GetHandTypeJoker(Hand const &hand);

struct Hand
{
    std::string_view cardsText_;
    std::array<char, 5> cards{};
    std::array<int, 15> counts{};
    bool hasJoker = false;
    HandType type;
    int bid = 0;

    Hand(std::string_view cardsText, bool joker)
        : cardsText_{cardsText}
        , hasJoker{joker}
    {
        auto const parts = Split(cardsText, ' ');
        size_t index = 0;

        for (auto c : parts[0])
        {
            auto const value = CardToValue(c, joker);
            cards[index++] = static_cast<char>('a' + value);
            counts[static_cast<size_t>(value)]++;
        }

        type = counts[1] != 0 ? GetHandTypeJoker(*this) : GetHandType(*this);
        bid = svtoi(parts[1]);
    }

    bool operator<(Hand const other) const
    {
        if (type == other.type)
        {
            return cards < other.cards;
        }

        return type < other.type;
    }
};

static std::pair<bool, int> CountSameCards(Hand const &hand, int nb, int exclude)
{
    int index = 0;

    for (auto c : hand.counts)
    {
        if (c == nb && exclude != index)
        {
            return {true, index};
        }

        ++index;
    }

    return {false, 0};
}

static HandType GetHandType(Hand const &hand)
{
    if (CountSameCards(hand, 5).first)
        return HandType::FiveOfAKind;

    if (CountSameCards(hand, 4).first)
        return HandType::FourOfAKind;

    if (CountSameCards(hand, 3).first)
        return CountSameCards(hand, 2).first ? HandType::FullHouse : HandType::ThreeOfAKind;

    if (auto pair1 = CountSameCards(hand, 2); pair1.first)
        return CountSameCards(hand, 2, pair1.second).first ? HandType::TwoPair : HandType::OnePair;

    return HandType::HighCard;
}

static HandType GetHandTypeJoker(Hand const &hand)
{
    std::set<HandType> types;

    CombinationsWithReplacement(std::span{"23456789TQKA"sv}, static_cast<size_t>(hand.counts[1]),
        [&](auto const &replacements)
        {
            std::string newCards{hand.cardsText_};
            auto iter = begin(replacements);

            for (char &c : newCards)
            {
                if (c == 'J')
                {
                    c = *iter++;
                }
            }

            Hand newHand(newCards, false);
            types.insert(GetHandType(newHand));
            return true;
        });

    return *types.rbegin();
}

static auto GetTotalWinnings(std::string_view text)
{
    std::vector<Hand> hands;
    for (auto line : Split(text))
    {
        hands.emplace_back(line, false);
    }

    std::sort(begin(hands), end(hands));

    int total = 0;
    auto const len = hands.size();
    for (size_t i = 0; i != len; ++i)
    {
        total += hands[i].bid * static_cast<int>(i + 1);
    }

    return total;
}

static auto Part1()
{
    return GetTotalWinnings(GetInput());
}

static auto GetTotalWinningsWithJoker(std::string_view text)
{
    std::vector<Hand> hands;
    for (auto line : Split(text))
    {
        hands.emplace_back(line, true);
    }

    std::sort(begin(hands), end(hands));

    int total = 0;
    auto const len = hands.size();
    for (size_t i = 0; i != len; ++i)
    {
        total += hands[i].bid * static_cast<int>(i + 1);
    }

    return total;
}

static auto Part2()
{
    return GetTotalWinningsWithJoker(GetInput());
}

int main()
{
    // https://adventofcode.com/2023/day/7
    fmt::print("Day 7, 2023: Camel Cards\n");

    Assert(6440 == GetTotalWinnings(example::hands));
    Assert(5905 == GetTotalWinningsWithJoker(example::hands));

    auto const part1 = Part1();
    fmt::print("  Part 1: {}\n", part1);
    Assert(253910319 == part1);

    auto const part2 = Part2();
    fmt::print("  Part 2: {}\n", part2);
    Assert(254083736 == part2);
}
