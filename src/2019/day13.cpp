#include "day13.hpp"

#include "../common.hpp"
#include "../common/intcode.hpp"

enum class Tile
{
    Empty,  // No game object appears in this tile.
    Wall,   // Walls are indestructible barriers.
    Block,  // Blocks can be broken by the ball.
    Paddle, // The paddle is indestructible.
    Ball,   // The ball moves diagonally and bounces off objects.
};

static void Part1()
{
    Point2d pos;
    std::unordered_map<Point2d, Tile> grid;

    auto setX = [&](Int value)
    {
        pos.x = static_cast<int>(value);
    };

    auto setY = [&](Int value)
    {
        pos.y = static_cast<int>(value);
    };

    auto setTitle = [&](Int value)
    {
        grid[pos] = static_cast<Tile>(value);
    };

    std::array<std::function<void(Int)>, 3> states = {setX, setY, setTitle};

    Intcode::Run(ParseInputNumbers<Int, ','>(), nullptr,
        [&](Int value)
        {
            states.front()(value);
            std::rotate(begin(states), begin(states) + 1, end(states));
        });

    auto result = std::count_if(begin(grid), end(grid),
        [](auto const &elem)
        {
            return elem.second == Tile::Block;
        });

    std::println("  Part1: {}", result);
    Assert(expected::part1 == result);
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

static void Part2(bool render)
{
    Intcode cpu(ParseInputNumbers<Int, ','>());
    cpu.WriteMemory(0, 2);

    Point2d inputPos;
    std::unordered_map<Point2d, Tile> grid;
    Int score = 0;
    Point2d ballPos;
    Point2d paddlePos;
    Int nextInput = 0;
    Point2d min;
    Point2d max;

    auto setX = [&](Int value)
    {
        inputPos.x = static_cast<int>(value);
        min.x = std::min(min.x, inputPos.x);
        max.x = std::max(max.x, inputPos.x);
    };

    auto setY = [&](Int value)
    {
        inputPos.y = static_cast<int>(value);
        min.y = std::min(min.y, inputPos.y);
        max.y = std::max(max.y, inputPos.y);
    };

    auto setTitle = [&](Int value)
    {
        if (inputPos.x == -1 && inputPos.y == 0)
        {
            score = value;

            if (render)
            {
                std::print(CSI("24;1H") "SCORE: {}", score);
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
                std::print(CSI("{};{}H") "{}", inputPos.y + 1, inputPos.x + 1, GetTile(tile));
            }

            grid[inputPos] = tile;
        }
    };

    cpu.SetInput(
        [&]()
        {
            return std::exchange(nextInput, 0);
        });

    std::array<std::function<void(Int)>, 3> states = {setX, setY, setTitle};
    cpu.SetOutput(
        [&](Int value)
        {
            states.front()(value);
            std::rotate(begin(states), begin(states) + 1, end(states));
        });

    if (render)
    {
        // Switches to a new alternate screen buffer.
        std::print(CSI("?1049h") CSI("?25l"));
    }

    cpu.Run();

    if (render)
    {
        // Switches back to the main buffer.
        std::print(CSI("?1049l") CSI("?25h"));
    }

    std::println("  Part2: {}", score);
    Assert(expected::part2 == score);
}

int main()
{
    bool const isTerminal = IsTerminal(stdout);
    std::println("Day 13, 2019: Care Package");

    Part1();

    // NOLINTNEXTLINE: cppcoreguidelines-pro-bounds-pointer-arithmetic
    bool const render = isTerminal;
    Part2(render);
}
