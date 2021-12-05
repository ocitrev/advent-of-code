#include "day04.hpp"
#include "../common/assert.hpp"
#include "../common/string.hpp"
#include <fmt/format.h>
#include <numeric>
#include <span>
#include <vector>

struct Cell
{
    int number = 0;
    bool hit = false;
};

struct Board
{
    std::array<std::array<Cell, 5>, 5> board;

    void Parse(std::string_view boardText)
    {
        auto row = begin(board);

        for (auto line : Split(boardText, '\n'))
        {
            auto col = begin(*row);

            for (auto number : Split(line, ' '))
            {
                col->number = svtoi(number);
                ++col;
            }

            ++row;
        }
    }

    bool HasRow() const
    {
        return (board[0][0].hit && board[0][1].hit && board[0][2].hit && board[0][3].hit && board[0][4].hit)
               || (board[1][0].hit && board[1][1].hit && board[1][2].hit && board[1][3].hit && board[1][4].hit)
               || (board[2][0].hit && board[2][1].hit && board[2][2].hit && board[2][3].hit && board[2][4].hit)
               || (board[3][0].hit && board[3][1].hit && board[3][2].hit && board[3][3].hit && board[3][4].hit)
               || (board[4][0].hit && board[4][1].hit && board[4][2].hit && board[4][3].hit && board[4][4].hit);
    }

    bool HasColumn() const
    {
        return (board[0][0].hit && board[1][0].hit && board[2][0].hit && board[3][0].hit && board[4][0].hit)
               || (board[0][1].hit && board[1][1].hit && board[2][1].hit && board[3][1].hit && board[4][1].hit)
               || (board[0][2].hit && board[1][2].hit && board[2][2].hit && board[3][2].hit && board[4][2].hit)
               || (board[0][3].hit && board[1][3].hit && board[2][3].hit && board[3][3].hit && board[4][3].hit)
               || (board[0][4].hit && board[1][4].hit && board[2][4].hit && board[3][4].hit && board[4][4].hit);
    }

    int GetScore() const
    {
        int score = 0;
        for (auto const &row : board)
        {
            for (auto const &col : row)
            {
                if (not col.hit)
                    score += col.number;
            }
        }

        return score;
    }

    bool HasBingo() const
    {
        return HasRow() || HasColumn();
    }

    bool Draw(int number)
    {
        for (auto &row : board)
        {
            for (auto &[n, hit] : row)
            {
                if (n == number)
                {
                    hit = true;
                }
            }
        }

        return HasBingo();
    }
};

static std::vector<Board> ParseBoards(std::string_view boardsText)
{
    std::vector<Board> boards;

    for (auto boardText : Split(boardsText, "\n\n"))
    {
        boards.emplace_back().Parse(boardText);
    }

    return boards;
}

static int Part1(std::span<int const> draw, std::string_view boardsText)
{
    std::vector<Board> boards = ParseBoards(boardsText);

    for (int n : draw)
    {
        for (auto &b : boards)
        {
            if (b.Draw(n))
            {
                return b.GetScore() * n;
            }
        }
    }

    return 0;
}

static int Part2(std::span<int const> draw, std::string_view boardsText)
{
    std::vector<Board> boards = ParseBoards(boardsText);
    Board *lastWinnerBoard = nullptr;
    int lastWinnerNumber = 0;

    for (int n : draw)
    {
        for (auto &b : boards)
        {
            if (b.HasBingo())
                continue;

            if (b.Draw(n))
            {
                lastWinnerBoard = &b;
                lastWinnerNumber = n;
            }
        }
    }

    return lastWinnerBoard->GetScore() * lastWinnerNumber;
}

int main()
{
    // https://adventofcode.com/2021/day/4
    fmt::print("Day 4, 2021: \n");

    Assert(4512 == Part1(example::draw, example::boards));
    Assert(1924 == Part2(example::draw, example::boards));

    auto const part1 = Part1(input::draw, input::boards);
    fmt::print("  Part 1: {}\n", part1);
    Assert(29440 == part1);

    auto const part2 = Part2(input::draw, input::boards);
    fmt::print("  Part 2: {}\n", part2);
    Assert(13884 == part2);
}
