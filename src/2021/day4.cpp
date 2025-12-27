#include "day4.hpp"

#include "../common.hpp"

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
                if (number.empty())
                {
                    continue;
                }

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
                {
                    score += col.number;
                }
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

struct Input
{
    std::vector<int> draw;
    std::string_view boards;

    Input()
    {
        std::string_view input = GetInput();
        auto const pos = input.find("\n\n");
        ParseDraw(input.substr(0, pos));
        boards = input.substr(pos + 2);
    }

    void ParseDraw(std::string_view numbers)
    {
        for (auto number : Split(numbers, ','))
        {
            draw.push_back(svtoi(number));
        }
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

static int Part1(Input const &input)
{
    return Part1(input.draw, input.boards);
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
            {
                continue;
            }

            if (b.Draw(n))
            {
                lastWinnerBoard = &b;
                lastWinnerNumber = n;
            }
        }
    }

    return lastWinnerBoard->GetScore() * lastWinnerNumber;
}

static int Part2(Input const &input)
{
    return Part2(input.draw, input.boards);
}

int main()
{
    // https://adventofcode.com/2021/day/4
    std::println("Day 4, 2021: Giant Squid");

    Assert(4512 == Part1(example::draw, example::boards));
    Assert(1924 == Part2(example::draw, example::boards));

    auto const part1 = Part1(Input{});
    std::println("  Part 1: {}", part1);
    Assert(29'440 == part1);

    auto const part2 = Part2(Input{});
    std::println("  Part 2: {}", part2);
    Assert(13'884 == part2);
}
