#include "day13.hpp"
#include "common/intcode.hpp"
#include "common/main.hpp"
#include "common/point.hpp"
#include <array>
#include <cassert>
#include <iostream>
#include <map>

enum class Tile
{
    Empty,  // No game object appears in this tile.
    Wall,   // Walls are indestructible barriers.
    Block,  // Blocks can be broken by the ball.
    Paddle, // The paddle is indestructible.
    Ball,   // The ball moves diagonally and bounces off objects.
};

void Part1()
{
    Point pos;
    std::map<Point, Tile> grid;

    auto setX = [&](Int value) {
        pos.x = static_cast<int>(value);
    };

    auto setY = [&](Int value) {
        pos.y = static_cast<int>(value);
    };

    auto setTitle = [&](Int value) {
        grid[pos] = static_cast<Tile>(value);
    };

    std::array<std::function<void(Int)>, 3> states = {setX, setY, setTitle};

    Intcode::Run(input::code, nullptr, [&](Int value) {
        states.front()(value);
        std::rotate(begin(states), begin(states) + 1, end(states));
    });

    auto result = std::count_if(begin(grid), end(grid), [](auto const &elem) {
        return elem.second == Tile::Block;
    });

    std::cout << "Part1: " << result << '\n';
    assert(expected::part1 == result);
}

static std::string GetTile(Tile tile)
{
    switch (tile)
    {
    case Tile::Wall:
        return "█";
    case Tile::Block:
        return "▪";
    case Tile::Paddle:
        return "▔";
    case Tile::Ball:
        return "●";
    default:
        return " ";
    }
}

static std::ostream & RenderAt(std::ostream &out, int x, int y)
{
    if (x > 0 && y > 0)
    {
        out << "\033[u\033[" << y << "B\033[" << x << "C";
    }
    else if (x > 0)
    {
        out << "\033[u\033[" << x << "C";
    }
    else if (y > 0)
    {
        out << "\033[u\033[" << y << "B";
    }
    else
    {
        out << "\033[u";
    }

    return out;
}

void Part2(bool render)
{
    Intcode cpu(input::code);
    cpu.WriteMemory(0, 2);

    Point inputPos;
    std::map<Point, Tile> grid;
    Int score = 0;
    Point ballPos;
    Point paddlePos;
    Int nextInput = 0;
    Point min;
    Point max;

    auto setX = [&](Int value) {
        inputPos.x = static_cast<int>(value);
        min.x = std::min(min.x, inputPos.x);
        max.x = std::max(max.x, inputPos.x);
    };

    auto setY = [&](Int value) {
        inputPos.y = static_cast<int>(value);
        min.y = std::min(min.y, inputPos.y);
        max.y = std::max(max.y, inputPos.y);
    };

    auto setTitle = [&](Int value) {
        if (inputPos.x == -1 && inputPos.y == 0)
        {
            score = value;

            if (render)
            {
                RenderAt(std::cout, 0, 23) << "SCORE: " << score;
            }
        }
        else
        {
            Tile const tile = static_cast<Tile>(value);

            if (tile == Tile::Ball)
            {
                ballPos = inputPos;

                if (paddlePos.x < ballPos.x)
                {
                    nextInput = 1;
                }
                else if (paddlePos.x > ballPos.x)
                {
                    nextInput = -1;
                }
            }
            else if (tile == Tile::Paddle)
            {
                paddlePos = inputPos;
            }

            if (render)
            {
                RenderAt(std::cout, inputPos.x, inputPos.y) << GetTile(tile);
            }

            grid[inputPos] = tile;
        }
    };

    cpu.SetInput([&]() {
        return std::exchange(nextInput, 0);
    });

    std::array<std::function<void(Int)>, 3> states = {setX, setY, setTitle};
    cpu.SetOutput([&](Int value) {
        states.front()(value);
        std::rotate(begin(states), begin(states) + 1, end(states));
    });

    if (render)
    {
        std::cout << "Part2:\n\033[s";
        std::cout << "\033[?25l";
    }

    cpu.Run();

    if (render)
    {
        RenderAt(std::cout, 0, max.y + 1) << "\033[?25h\n";
    }
    else
    {
        std::cout << "Part2: " << score << '\n';
    }

    assert(expected::part2 == score);
}

void Main()
{
    std::cout << "Day 13: Care Package\n";

    Part1();
    Part2(false);

    GetTile(Tile::Ball);
    
}
