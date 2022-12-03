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
        return 3;

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

constexpr int Score(std::string_view round)
{
    return Score(Parse(round[0]), Parse(round[2]));
}

static_assert(Score(Shape::Paper, Shape::Rock) == 1);
static_assert(Score(Shape::Scissors, Shape::Paper) == 2);
static_assert(Score(Shape::Rock, Shape::Scissors) == 3);
static_assert(Score(Shape::Rock, Shape::Rock) == 4);
static_assert(Score(Shape::Paper, Shape::Paper) == 5);
static_assert(Score(Shape::Scissors, Shape::Scissors) == 6);
static_assert(Score(Shape::Scissors, Shape::Rock) == 7);
static_assert(Score(Shape::Rock, Shape::Paper) == 8);
static_assert(Score(Shape::Paper, Shape::Scissors) == 9);
static_assert(Score("B X") == 1);
static_assert(Score("C Y") == 2);
static_assert(Score("A Z") == 3);
static_assert(Score("A X") == 4);
static_assert(Score("B Y") == 5);
static_assert(Score("C Z") == 6);
static_assert(Score("C X") == 7);
static_assert(Score("A Y") == 8);
static_assert(Score("B Z") == 9);

constexpr Shape WinnerFor(Shape input)
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

constexpr Shape LoserFor(Shape input)
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

static_assert(Shape::Rock == WinnerFor(Shape::Scissors));
static_assert(Shape::Paper == WinnerFor(Shape::Rock));
static_assert(Shape::Scissors == WinnerFor(Shape::Paper));
static_assert(Shape::Rock == LoserFor(Shape::Paper));
static_assert(Shape::Paper == LoserFor(Shape::Scissors));
static_assert(Shape::Scissors == LoserFor(Shape::Rock));

constexpr int Score2(std::string_view round)
{
    Shape const player1 = Parse(round[0]);
    Shape player2;

    switch (round[2])
    {
    case 'X':
        player2 = LoserFor(player1);
        break;
    case 'Y':
        player2 = player1;
        break;
    case 'Z':
        player2 = WinnerFor(player1);
        break;
    }

    return Score(player1, player2);
}

static int Part1(std::string_view lines)
{
    int total = 0;

    for (auto line : Split(lines, '\n'))
    {
        total += Score(line);
    }

    return total;
}

static int Part2(std::string_view lines)
{
    int total = 0;

    for (auto line : Split(lines, '\n'))
    {
        total += Score2(line);
    }

    return total;
}

int main()
{
    // https://adventofcode.com/2022/day/2
    fmt::print("Day 2, 2022: Rock Paper Scissors\n");

    Assert(15 == Part1(example::lines));
    Assert(12 == Part2(example::lines));

    auto const part1 = Part1(input::lines);
    fmt::print("  Part 1: {}\n", part1);
    Assert(12740 == part1);

    auto const part2 = Part2(input::lines);
    fmt::print("  Part 2: {}\n", part2);
    Assert(11980 == part2);
}