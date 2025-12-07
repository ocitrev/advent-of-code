#include "day2.hpp"

#include "../common.hpp"

enum struct Shape : int
{
    Rock = 1,
    Paper = 2,
    Scissors = 3,
};

constexpr Shape Parse(char c)
{
    switch (c)
    {
        case 'A':
        case 'X':
            return Shape::Rock;
        case 'B':
        case 'Y':
            return Shape::Paper;
        case 'C':
        case 'Z':
            return Shape::Scissors;
    }

    throw std::invalid_argument{"c"};
}

constexpr int Game(Shape player1, Shape player2)
{
    if (player1 == player2)
    {
        return 3;
    }

    switch (player1)
    {
        case Shape::Rock:
            return player2 == Shape::Paper ? 6 : 0;
        case Shape::Paper:
            return player2 == Shape::Scissors ? 6 : 0;
        case Shape::Scissors:
            return player2 == Shape::Rock ? 6 : 0;
    }

    throw std::invalid_argument("player1");
}

constexpr int Score(Shape player1, Shape player2)
{
    return Game(player1, player2) + static_cast<int>(player2);
}

constexpr int Score(char player1, char player2)
{
    return Score(Parse(player1), Parse(player2));
}

constexpr int ScoreLine(std::string_view round)
{
    return Score(Parse(round[0]), Parse(round[2]));
}

static_assert(1 == Score(Shape::Paper, Shape::Rock));
static_assert(2 == Score(Shape::Scissors, Shape::Paper));
static_assert(3 == Score(Shape::Rock, Shape::Scissors));
static_assert(4 == Score(Shape::Rock, Shape::Rock));
static_assert(5 == Score(Shape::Paper, Shape::Paper));
static_assert(6 == Score(Shape::Scissors, Shape::Scissors));
static_assert(7 == Score(Shape::Scissors, Shape::Rock));
static_assert(8 == Score(Shape::Rock, Shape::Paper));
static_assert(9 == Score(Shape::Paper, Shape::Scissors));
static_assert(1 == ScoreLine("B X"));
static_assert(2 == ScoreLine("C Y"));
static_assert(3 == ScoreLine("A Z"));
static_assert(4 == ScoreLine("A X"));
static_assert(5 == ScoreLine("B Y"));
static_assert(6 == ScoreLine("C Z"));
static_assert(7 == ScoreLine("C X"));
static_assert(8 == ScoreLine("A Y"));
static_assert(9 == ScoreLine("B Z"));

constexpr Shape WinnerAgainst(Shape input)
{
    switch (input)
    {
        case Shape::Rock:
            return Shape::Paper;
        case Shape::Paper:
            return Shape::Scissors;
        case Shape::Scissors:
            return Shape::Rock;
    }

    throw std::invalid_argument("input");
}

constexpr Shape LoserAgainst(Shape input)
{
    switch (input)
    {
        case Shape::Rock:
            return Shape::Scissors;
        case Shape::Paper:
            return Shape::Rock;
        case Shape::Scissors:
            return Shape::Paper;
    }

    throw std::invalid_argument("input");
}

static_assert(Shape::Rock == WinnerAgainst(Shape::Scissors));
static_assert(Shape::Paper == WinnerAgainst(Shape::Rock));
static_assert(Shape::Scissors == WinnerAgainst(Shape::Paper));
static_assert(Shape::Rock == LoserAgainst(Shape::Paper));
static_assert(Shape::Paper == LoserAgainst(Shape::Scissors));
static_assert(Shape::Scissors == LoserAgainst(Shape::Rock));

constexpr Shape Parse2(char letter, Shape input)
{
    if (letter == 'X')
    {
        return LoserAgainst(input);
    }

    if (letter == 'Z')
    {
        return WinnerAgainst(input);
    }

    return input;
}

constexpr int Score2(std::string_view round)
{
    Shape const player1 = Parse(round[0]);
    Shape const player2 = Parse2(round[2], player1);
    return Score(player1, player2);
}

static_assert(4 == Score2("A Y"));
static_assert(1 == Score2("B X"));
static_assert(7 == Score2("C Z"));

static int Part1(std::string_view lines)
{
    return TransformReduceLines(lines, 0, std::plus{}, &ScoreLine);
}

static int Part2(std::string_view lines)
{
    return TransformReduceLines(lines, 0, std::plus{}, &Score2);
}

int main()
{
    // https://adventofcode.com/2022/day/2
    std::print("Day 2, 2022: Rock Paper Scissors\n");

    Assert(15 == Part1(example::lines));
    Assert(12 == Part2(example::lines));

    auto const part1 = Part1(GetInput());
    std::print("  Part 1: {}\n", part1);
    Assert(12'740 == part1);

    auto const part2 = Part2(GetInput());
    std::print("  Part 2: {}\n", part2);
    Assert(11'980 == part2);
}
